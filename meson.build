project('petraea', 'c',
  version : '1.0.0',
  license : 'MIT',
  default_options : 'optimization=0')

sljit_dep = dependency('sljit')

inc = include_directories('include')

#################################################
# Sources
#################################################

petraea_sources = [
  'source/cpu.c',
  'source/decoder.c',
  'source/util.c',
  'source/emitter-sljit.c',
  'source/codecache.c'
]

test_sources = [
  'test/cpu.test.c',
  'test/decoder.test.c',
  'test/util.test.c',
  'test/codecache.test.c'
]

## CPU specific optimisations
if host_machine.cpu_family() == 'x86_64'
  add_project_arguments('-DHOST_ARCH_X64', language : 'c')

  petraea_sources += [
    'source/x64/executor_x64.S',
    'source/x64/executor_x64.c'
  ]
  test_sources += [
    'test/x64/executor_x64.test.c'
  ]
else
  # Fallback to pure C executor
  petraea_sources += [
    'source/executor.c'
  ]
  test_sources += [
    'test/executor.test.c'
  ]
endif

#################################################
# Generate struct offset header for assembly code
#################################################

genhdr = executable('genhdr', ['genhdr.c'],
  include_directories : inc)

struct_offsets_h = custom_target(
  'struct_offsets.h',
  output: 'struct_offsets.h',
  command: [genhdr],
  capture: true
)

petraea_sources += struct_offsets_h

#################################################
# Library
#################################################

install_headers([
  'include/cpu.h',
  'include/instr.h'
])

libpetraea = shared_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  dependencies : [sljit_dep],
  install : true)

libpetraea_static = static_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  dependencies : [sljit_dep],
  install : true)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : libpetraea,
                 version : '1.0',
                 name : 'libpetraea',
                 filebase : 'petraea',
                 description : 'An ARM emulator core.')

libpetraea_dep = declare_dependency(
  include_directories : inc,
  link_with : libpetraea)

#################################################
# Unit tests
#################################################

criterion_dep = dependency('criterion')

testexe = executable('petraea.test', test_sources,
  include_directories : inc,
  dependencies : [criterion_dep, sljit_dep],
  link_with : libpetraea)
test('petraea test', testexe)

