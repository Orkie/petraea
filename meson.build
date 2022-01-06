project('petraea', 'c',
  version : '1.0.0',
  license : 'MIT')

inc = include_directories('include')

#################################################
# Sources
#################################################

petraea_sources = [
  'source/cpu.c',
  'source/decoder.c',
  'source/executor.c',
  'source/util.c'
]

test_sources = [
  'test/cpu.test.c',
  'test/decoder.test.c',
  'test/executor.test.c',
  'test/util.test.c'
]

#################################################
# Library
#################################################

install_headers([
  'include/cpu.h',
  'include/instr.h'
])

petraealib = shared_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  install : true)

petraealib_static = static_library(
  'petraea',
  petraea_sources,
  include_directories : inc,
  install : true)

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : petraealib,
                 version : '1.0',
                 name : 'libpetraea',
                 filebase : 'petraea',
                 description : 'An ARM emulator core.')

#################################################
# Unit tests
#################################################

criterion_dep = dependency('criterion')

testexe = executable('petraea.test', test_sources,
                     include_directories : inc,
		     dependencies : criterion_dep,
		     link_with : petraealib)
test('petraea test', testexe)

