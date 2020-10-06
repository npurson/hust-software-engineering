#!/bin/env python3
import argparse
import configparser
import csv
import json
import operator
import os
import re
import subprocess
import time  # time_ns() is new in python3.7
from subprocess import Popen
import signal

from openpyxl import Workbook
from openpyxl.styles import PatternFill
from tqdm import tqdm
from tqdm.contrib.concurrent import process_map, thread_map

# basic config here
CONFIG_JSON_NAME = "config.json"
CAPTURE_CMD = (
    'preset', 'user', 'map', 'fund', 'credit',
    'gift', 'bomb', 'barrier', 'userloc', 'nextuser'
)
# end config

green = PatternFill(start_color="7FFF00",
                    end_color="7FFF00", fill_type="solid")
red = PatternFill(start_color="FF0000", end_color="FF0000", fill_type="solid")

args = argparse.ArgumentParser()
args.add_argument('-d', '--dir', help='testcase dir',
                  default='.', type=str)
args.add_argument('-w', '--workers',
                  help='work process number', default=4, type=int)
args.add_argument('-n', '--name', help='process name',
                  default='../richman/richman', type=str)
args.add_argument('-e', '--export', help='export to file',
                  default=True, type=bool)
args.add_argument('-f', '--exportfile', help='export file name',
                  default='report.csv', type=str)
args = args.parse_args()
basedir = args.dir

kv_pat = re.compile(r'([a-zA-Z]+)(?:\s+(\w+))(?:\s+(\w+))?(?:\s+(\w+))?')

test_list = dict()


def trim_line(line):
    if not line:
        return ''

    line = re.sub(r'(#|\/\/).*$', ' ', line)  # comment
    line = re.sub(r'\s+', ' ', line.strip())  # strip and merge empty char
    if not line:
        return ''
    line = line.lower()  # lowercase

    if not any(line.startswith(c + ' ') for c in CAPTURE_CMD):
        return ''  # check first word

    return line


def parse_key_value_pairs(lines):
    ret = []
    for line in lines:
        line = trim_line(line)
        if not line:
            continue
        mat = kv_pat.match(line)
        if mat:
            ret.append('+'.join((e for e in mat.groups() if e)))
        else:
            print('error at: ', line)
            # assert False
    # print(ret)
    ret.sort()
    return ret


def check(case):
    '''
        case[0] = test name
        case[1] = testcase path
    '''
    with open(os.path.join(basedir, case[1]+'.in'), 'r', encoding='UTF-8') as fin:
        testcase_input = fin.read()
        pass
    with open(os.path.join(basedir, case[1]+'.out'), 'r', encoding='UTF-8') as f:
        testcase_answer = f.read()
    proc = Popen(
        args.name, shell=True,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        universal_newlines=True
    )

    output = ''
    try:
        t = time.time()
        _, output = proc.communicate(testcase_input + '\n', timeout=10)
        t = time.time() - t
    except subprocess.TimeoutExpired:
        t = time.time() - t
        # proc.terminate()
        # os.killpg(proc.pid, signal.SIGKILL)

    # print('output:', output)
    if output:
        out_kv = parse_key_value_pairs(output.splitlines())
    else:
        out_kv = list()
    ans_kv = parse_key_value_pairs(testcase_answer.splitlines())
    # print(ans_kv, out_kv)
    return (
        case[0],
        os.path.basename(case[1]),
        # (not any((e not in out_kv for e in ans_kv))) and
        # (not any((e not in ans_kv for e in out_kv))),
        operator.eq(out_kv, ans_kv),
        t * 1000
    )


def add_case(testname, casedir):
    print('case <', testname, '>', casedir)
    if testname not in test_list.keys():
        test_list[testname] = list()
    test_list[testname].append(casedir)
    return


def get_all_case_filename(dir):
    return list(
        set(
            os.path.splitext(casename)[0]
            for casename in os.listdir(dir)
            if os.path.splitext(casename)[1] in ['.in', '.out']
        )
    )


def run_dir(currdir, basename='', maxlevel=1000):
    dir_list = os.listdir(currdir)
    if CONFIG_JSON_NAME in dir_list:
        # with json configuration file
        with open(os.path.join(currdir, CONFIG_JSON_NAME), 'r', encoding='UTF-8') as conf:
            conf = json.load(conf)

        name = os.path.basename(currdir)
        if 'name' in conf.keys():
            name = conf['name']

        if 'maxlevel' in conf.keys():
            maxlevel = min(maxlevel, int(conf['maxlevel']))

        if 'import' in conf.keys():
            # import other dir
            for i in conf['import']:
                d = os.path.join(currdir, i)
                if not os.path.isdir(d):
                    continue  # is not a dir
                if basename:
                    n = '%s.%s' % (basename, name)
                else:
                    n = name
                # print("import>", d, ',', n, ',', maxlevel)
                run_dir(d, n, maxlevel)
            pass

        if 'case' in conf.keys():
            if basename:
                n = '%s.%s' % (basename, name)
            else:
                n = name
            for item in conf['case']:
                if int(item[1]) <= maxlevel:
                    add_case(n, os.path.join(currdir, item[0]))

        pass
    elif maxlevel >= 0:
        # only testcases in dir
        c = get_all_case_filename(currdir)
        for item in c:
            add_case(
                '%s.%s' % (basename, os.path.basename(currdir)),
                os.path.join(currdir, item)
            )
    return


run_dir(basedir, '')
# print(test_list)

testcases = list()
keys = list(test_list.keys())
keys.sort()
for k in keys:
    for v in test_list[k]:
        testcases.append((k, v))
testcases = set(testcases)
# print(testcases)

print('run', args.name)
res = thread_map(check, testcases, max_workers=args.workers)
# res.sort(key=lambda x: (x[0], x[1]))
# print(res)

test_res = dict()
for item in res:
    if item[0] not in test_res:
        test_res[item[0]] = list()
    test_res[item[0]].append(item)
for item in test_res:
    test_res[item].sort(key=lambda x: x[0])
print(test_res)


if args.export:
    if args.exportfile.endswith('xlsx'):
        wb = Workbook()
        ws = wb.active
        ws.cell(1, 1).value = 'testcase'
        ws.cell(1, 2).value = 'status'
        ws.cell(1, 3).value = 'timeuse(ms)'
        for idx, (testname, status, timeuse) in enumerate(res, 2):
            ws.cell(idx, 1).value = testname
            ws.cell(idx, 2).value = 'PASS' if status else 'FAIL'
            ws.cell(idx, 2).fill = green if status else red
            ws.cell(idx, 3).value = timeuse
        wb.save(args.exportfile)
    elif args.exportfile.endswith('csv'):
        with open(args.exportfile, 'w') as f:
            writer = csv.writer(f)
            for test in test_res:
                cnt = 0
                for item in test_res[test]:
                    if item[2]:
                        cnt = cnt + 1

                writer.writerow([
                    'testname',
                    test,
                    '%d\\%d' % (cnt, len(test_res[test]))
                ])
                writer.writerow(['testname', 'testcase', 'status', 'time(ms)'])
                writer.writerows(test_res[test])
                writer.writerow([])
            pass  # with open as f:
