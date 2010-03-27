top = '.'

def configure(conf):
	conf.check_tool('compiler_cxx')
	
	conf.env.CXXFLAGS = ['-g3', '-pedantic', '-Werror']
	
	conf.check_cfg(package='xcb', args="--cflags --libs", mandatory=True)
	
	conf.env.LIB_BOOST_SIGNALS=['boost_signals']

def set_options(opt):
	opt.tool_options('compiler_cxx')

def build(bld):
	bld.add_subdirs('manix-wm')