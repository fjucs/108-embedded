#!/usr/bin/env python3
import sys, os

symToVal = {'⚫': 0, '⚪': 1}
target = 'show.h'
entry = 'please_start_your_performance'
interval = 200000

def gen_line(h, time):
	return \
	'''    GPIO_PTB_GPIO = {}; // {}\n    delay1({});\n'''.format(hex(h), bin(h), str(time))

def gen_file(stage):
	l = None
	t = None
	with open(stage, 'r', encoding='utf-8') as f:
		t = int(f.readline())
		l = f.readlines()
	l = [x.rstrip('\n') for x in l]
	lhex = []

	for line in l:
		tmp = 0
		for c in line:
			print(c, end='')
			tmp <<= 1
			tmp |= symToVal[c]
		print(' = {}'.format(hex(tmp)))
		lhex.append(tmp)
	'''
	for i in lhex:
		print(gen_line(i, 2000))
	'''
	with open(target, 'a', encoding='utf-8') as f:
		f.write('inline void {}()\n{}'.format(stage, '{\n'))
		for _ in range(t):
			for i in lhex:
				f.write(gen_line(i, interval))
		f.write('}\n')

def gen_epilogue(flist):
	with open(target, 'a', encoding='utf-8') as f:
		f.write('inline void {}()\n'.format(entry))
		f.write('{\n')
		for i in flist:
			f.write('    {}();\n'.format(i))
		f.write('}\n')

if __name__ == '__main__':
	flist = ['stage1', 'stage2', 'stage3', 'stage4']

	try:
		os.remove(target)
	except FileNotFoundError:
		pass

	for i in flist:
		gen_file(i)
	gen_epilogue(flist)

	print('Add the next line to your head of main.c', end='\n\n')
	print('#include "{}"'.format(target), end='\n\n')
	print('Add the following line in your main func:', end='\n\n')
	print('{}();'.format(entry))