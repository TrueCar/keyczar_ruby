require 'mkmf'

extension_name = 'keyczar_ruby'

dir_config(extension_name)
dir_config("keyczar", "/usr/local/include", "/usr/local/lib")

have_library("keyczar") or raise RuntimeError, <<-MSG
Could not find keyczar C++ library. Are you sure you have it installed?
**
** Source files can be found here:
** http://code.google.com/p/keyczar/downloads/list
**
** see README.rdoc for more details
MSG

create_makefile(extension_name)
