project(
  'advent',
  'c',
  version: '0.1',
  default_options: [
    'warning_level=3',
    'c_std=c2x',
    'optimization=3',
    'b_lto=true',
    'buildtype=release',
    # 'buildtype=debug',
  ],
)

fs = import('fs')

c_args = [
  '-fsanitize-recover=all',
  '-march=native',
]

link_args = ['-fsanitize-recover=all']

cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : true)

foreach year : [2021, 2022, 2023, 2025]
  foreach day : range(26)
    ystr = year.to_string()
    dstr = day.to_string()

    script_filename = ystr + '/d' + dstr + '.c'
    if not fs.exists(script_filename)
      continue
    endif

    input_filename = meson.current_source_dir() + '/' + ystr + '/input/d' + dstr + '.txt'

    executable(
      ystr + '_d' + dstr,
      script_filename,
      include_directories: ['include'],
      c_args: c_args + ['-DINPUT_FILENAME="' + input_filename + '"'],
      link_args: link_args,
      dependencies : m_dep
    )
  endforeach
endforeach
