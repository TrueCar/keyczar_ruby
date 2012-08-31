require 'mkmf'

extension_name = 'keyczar_ruby'

dir_config(extension_name)
dir_config("keyczar", "/usr/local/include", "/usr/local/lib")

have_library("keyczar") or raise

create_makefile(extension_name)
