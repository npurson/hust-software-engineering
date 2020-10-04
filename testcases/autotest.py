#!/bin/env python3
import argparse
import configparser
import csv
import os
import re
import subprocess
import time
from subprocess import Popen

from openpyxl import Workbook
from openpyxl.styles import PatternFill
from tqdm import tqdm
from tqdm.contrib.concurrent import process_map, thread_map

green = PatternFill(start_color="7FFF00",
                    end_color="7FFF00", fill_type="solid")
red = PatternFill(start_color="FF0000", end_color="FF0000", fill_type="solid")

args = argparse.ArgumentParser()
args.add_argument('-d', '--dir', help='testcase dir',
                  default='.', type=str)
args.add_argument('-w', '--workers',
                  help='work process number', default=4, type=int)
args.add_argument('-n', '--name', help='process name',
                  default='../richman', type=str)
args.add_argument('-e', '--export', help='export to file',
                  default=True, type=bool)
args.add_argument('-f', '--exportfile', help='export file name',
                  default='report.csv', type=str)
args = args.parse_args()
basedir = args.dir

all_testcases = os.listdir(basedir)
testcases = list(
    set((os.path.splitext(casename)[0] for casename in all_testcases if os.path.splitext(casename)[1] in ['.in', '.out'])))
testcases.sort()
for casename in testcases:
    assert casename+'.in' in all_testcases and casename+'.out' in all_testcases

kv_pat = re.compile(r'([a-zA-Z]+)(?:\s+(\w+))(?:\s+(\w+))?(?:\s+(\w+))?')


def prase_kv(lines):
    ret = []
    for line in lines:
        if not line:
            continue
        if '#' in line:
            line = line[:line.index('#')]
        mat = kv_pat.match(line)
        if mat:
            ret.append('_'.join((e for e in mat.groups() if e)))
        else:
            print('error at: ', line)
            assert False
    return ret


def check(casename):
    with open(os.path.join(basedir, casename+'.in'), 'r') as f:
        testcase_input = f.read()
    with open(os.path.join(basedir, casename+'.out'), 'r') as f:
        testcase_answer = f.read()
    proc = Popen(args.name, shell=True, stdout=subprocess.PIPE,
                 stderr=subprocess.PIPE)
    try:
        t = time.time_ns()
        _ , out = proc.communicate(testcase_input, timeout=5)
        t = time.time_ns()-t
    except subprocess.TimeoutExpired:
        t = time.time_ns()-t
    out_kv = prase_kv(out.decode().splitlines())
    ans_kv = prase_kv(testcase_answer.splitlines())
    return (casename, not any((e not in out_kv for e in ans_kv)), t/1e6)


res = thread_map(check, testcases, max_workers=args.workers)
res.sort(key=lambda x: x[0])
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
            writer.writerow(['testcase', 'status', 'timeuse(ms)'])
            writer.writerows(res)
