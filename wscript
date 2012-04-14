def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.env.append_unique('CXXFLAGS', ['-Wall', '-O3'])

def build(bld):
  ext = bld.new_task_gen("cxx", "shlib", "node_addon")
  ext.cxxflags = ["-g", "-Wall"]
  ext.source = "movefile.cc"
  ext.target = "movefile"
