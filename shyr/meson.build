includes = [include_directories('.')]
sources = ['shyr.c']

shyr_lib = static_library('shyr', sources, include_directories: includes)
shyr_dep = declare_dependency(sources: sources,include_directories: includes, link_with: shyr_lib)
