import os
import subprocess
import sys
import sysconfig
import platform

import SCons.Errors

SCons.Warnings.warningAsException(True)

# pending upstream fix - https://github.com/SCons/scons/issues/4461
#SetOption('warn', 'all')

TICI = False
AGNOS = TICI

Decider('MD5-timestamp')

SetOption('num_jobs', int(os.cpu_count()/2))

AddOption('--kaitai',
          action='store_true',
          help='Regenerate kaitai struct parsers')

AddOption('--asan',
          action='store_true',
          help='turn on ASAN')

AddOption('--ubsan',
          action='store_true',
          help='turn on UBSan')

AddOption('--coverage',
          action='store_true',
          help='build with test coverage options')

AddOption('--clazy',
          action='store_true',
          help='build with clazy')

AddOption('--compile_db',
          action='store_true',
          help='build clang compilation database')

AddOption('--ccflags',
          action='store',
          type='string',
          default='',
          help='pass arbitrary flags over the command line')

AddOption('--snpe',
          action='store_true',
          help='use SNPE on PC')

AddOption('--external-sconscript',
          action='store',
          metavar='FILE',
          dest='external_sconscript',
          help='add an external SConscript to the build')

AddOption('--pc-thneed',
          action='store_true',
          dest='pc_thneed',
          help='use thneed on pc')

AddOption('--minimal',
          action='store_false',
          dest='extras',
          default=os.path.exists(File('#.lfsconfig').abspath), # minimal by default on release branch (where there's no LFS)
          help='the minimum build to run openpilot. no tests, tools, etc.')

## Architecture name breakdown (arch)
## - larch64: linux tici aarch64
## - aarch64: linux pc aarch64
## - x86_64:  linux pc x64
## - Darwin:  mac x64 or arm64
real_arch = arch = 'aarch64'

lenv = {
  "PATH": "/home/fgx/.buildozer/android/platform/android-ndk-r25b/toolchains/llvm/prebuilt/linux-x86_64/bin/:/home/fgx/p4a2/venv/bin:/bin", #os.environ['PATH'],
  "LD_LIBRARY_PATH": ["/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/libs_collections/oscservice/arm64-v8a"],
  "PYTHONPATH": Dir("#").abspath + ':' + Dir(f"#third_party/acados").abspath,

  "ACADOS_SOURCE_DIR": Dir("#third_party/acados").abspath,
  "ACADOS_PYTHON_INTERFACE_PATH": Dir("#third_party/acados/acados_template").abspath,
  "TERA_PATH": Dir("#").abspath + f"/third_party/acados/x86_64/t_renderer"
}

cflags = ["--target=aarch64-linux-android31"]
cxxflags = ["--target=aarch64-linux-android31"]
cpppath = []
rpath = [] #lenv["LD_LIBRARY_PATH"].copy()

libpath = [
  # f"#third_party/acados/{arch}/lib",
  # f"#third_party/libyuv/{arch}/lib",

  # "/usr/lib",
  # "/usr/local/lib",
]


if GetOption('asan'):
  ccflags = ["-fsanitize=address", "-fno-omit-frame-pointer"]
  ldflags = ["-fsanitize=address"]
elif GetOption('ubsan'):
  ccflags = ["-fsanitize=undefined"]
  ldflags = ["-fsanitize=undefined"]
else:
  ccflags = []
  ldflags = ['-llog']

# Enable swaglog include in submodules
cflags += ['-DSWAGLOG="\\"common/swaglog.h\\""']
cxxflags += ['-DSWAGLOG="\\"common/swaglog.h\\""']

ccflags_option = GetOption('ccflags')
if ccflags_option:
  ccflags += ccflags_option.split(' ')

env = Environment(
  ENV=lenv,
  CCFLAGS=[
    "-g",
    "-fPIC",
    "-O2",
    "-Wunused",
    "-Werror",
    "-Wshadow",
    "-Wno-unknown-warning-option",
    "-Wno-deprecated-register",
    "-Wno-register",
    "-Wno-inconsistent-missing-override",
    "-Wno-c99-designator",
    "-Wno-reorder-init-list",
    "-Wno-error=unused-but-set-variable",
    "-Wno-vla-cxx-extension",
  ] + cflags + ccflags,

  CPPPATH=cpppath + [
    "#",
    "#third_party/acados/include",
    "#third_party/acados/include/blasfeo/include",
    "#third_party/acados/include/hpipm/include",
    "#third_party/catch2/include",
    "#third_party/libusb",
    "#third_party/libyuv/include",
    "#third_party/json11",
    "#third_party/linux/include",
    "#third_party/snpe/include",
    "#third_party/qrcode",
    "#third_party",
    "#cereal",
    "#msgq",
    "#opendbc/can",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/other_builds/capnp/arm64-v8a__ndk_target_24/capnp/c++/src/",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/other_builds/libzmq/arm64-v8a__ndk_target_24/libzmq/include/",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/other_builds/python3/arm64-v8a__ndk_target_24/python3/Include/",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/python-installs/oscservice/arm64-v8a/numpy/core/include/",
  ],

  CC='clang --target=aarch64-linux-android24',
  CXX='clang++ --target=aarch64-linux-android24',
  LINKFLAGS=ldflags,
  AR='llvm-ar',
  RANLIB='llvm-ranlib',

  RPATH=rpath,

  CFLAGS=["-std=gnu11"] + cflags,
  CXXFLAGS=["-std=c++1z"] + cxxflags,
  LIBPATH=libpath + [
    "#msgq_repo",
    "#third_party",
    "#third_party/json11",
    "#third_party/libusb/android/libs/arm64-v8a",
    "#selfdrive/pandad",
    "#common",
    "#rednose/helpers",
    "/home/fgx/.buildozer/android/platform/android-ndk-r25b/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/libs_collections/oscservice/arm64-v8a",
    "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/other_builds/python3/arm64-v8a__ndk_target_24/python3/android-build",
    "/home/fgx/.buildozer/android/platform/android-ndk-r25b/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/32",
  ],
  CYTHONCFILESUFFIX=".cpp",
  COMPILATIONDB_USE_ABSPATH=True,
  REDNOSE_ROOT="#",
  tools=["default", "cython", "compilation_db", "rednose_filter"],
  toolpath=["#rednose_repo/site_scons/site_tools"],
)

# Setup cache dir
cache_dir = '/data/scons_cache' if AGNOS else '/tmp/scons_cache'
CacheDir(cache_dir)
Clean(["."], cache_dir)

node_interval = 5
node_count = 0
def progress_function(node):
  global node_count
  node_count += node_interval
  sys.stderr.write("progress: %d\n" % node_count)

if os.environ.get('SCONS_PROGRESS'):
  Progress(progress_function, interval=node_interval)

# Cython build environment
py_include = "/home/fgx/p4a2/.buildozer/android/platform/build-arm64-v8a/build/other_builds/python3/arm64-v8a__ndk_target_24/python3/Include/" # sysconfig.get_paths()['include']
envCython = env.Clone()
envCython["CPPPATH"] += [py_include] #, np.get_include()]
envCython["CCFLAGS"] += ["-Wno-#warnings", "-Wno-shadow", "-Wno-deprecated-declarations"]
envCython["CCFLAGS"].remove("-Werror")

envCython["LIBS"] = ['python3.11']
if arch == "Darwin":
  envCython["LINKFLAGS"] = ["-bundle", "-undefined", "dynamic_lookup"] + darwin_rpath_link_flags
else:
  envCython["LINKFLAGS"] = ["-pthread", "-shared"]

Export('envCython')

# Qt build environment
qt_env = env.Clone()
qt_modules = ["Widgets", "Gui", "Core", "Network", "Concurrent", "Multimedia", "Quick", "Qml", "QuickWidgets", "Location", "Positioning", "DBus", "Xml"]


if GetOption("clazy"):
  checks = [
    "level0",
    "level1",
    "no-range-loop",
    "no-non-pod-global-static",
  ]
  qt_env['CXX'] = 'clazy'
  qt_env['ENV']['CLAZY_IGNORE_DIRS'] = qt_dirs[0]
  qt_env['ENV']['CLAZY_CHECKS'] = ','.join(checks)

Export('env', 'qt_env', 'arch', 'real_arch')

# Build common module
SConscript(['common/SConscript'])
Import('_common', '_gpucommon')

common = [_common, 'json11']
gpucommon = [_gpucommon]

Export('common', 'gpucommon')

# Build messaging (cereal + msgq + socketmaster + their dependencies)
SConscript(['msgq_repo/SConscript'])
SConscript(['cereal/SConscript'])
Import('socketmaster', 'msgq')
messaging = [socketmaster, msgq, 'zmq', 'capnp', 'kj',]
Export('messaging')


# # Build other submodules
SConscript([
  # 'body/board/SConscript',
  'opendbc/can/SConscript',
  # 'panda/SConscript',
])

# # Build rednose library
SConscript(['rednose/SConscript'])

# # Build system services
# SConscript([
#   'system/proclogd/SConscript',
#   'system/ubloxd/SConscript',
#   'system/loggerd/SConscript',
# ])
# if arch != "Darwin":
#   SConscript([
#     'system/sensord/SConscript',
#     'system/logcatd/SConscript',
#   ])

# if arch == "larch64":
#   SConscript(['system/camerad/SConscript'])

# # Build openpilot
# SConscript(['third_party/SConscript'])

SConscript(['selfdrive/SConscript'])

# if Dir('#tools/cabana/').exists() and GetOption('extras'):
#   SConscript(['tools/replay/SConscript'])
#   if arch != "larch64":
#     SConscript(['tools/cabana/SConscript'])

# external_sconscript = GetOption('external_sconscript')
# if external_sconscript:
#   SConscript([external_sconscript])
