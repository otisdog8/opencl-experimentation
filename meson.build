project('opencl-experimentation', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++14'])


opencl = dependency('OpenCL')

xxd = find_program('xxd')

gen_kernel = custom_target('gen-output', input : 'vecadd.cl', output : 'kernel.h', command : [xxd, '-i', '@INPUT@', '@OUTPUT@'])

executable('opencl-experimentation',
           ['main.cpp', gen_kernel],
           install : true,
           dependencies: opencl)

executable('opencl-info',
           'platforms.cpp',
           install : true,
           dependencies: opencl)
