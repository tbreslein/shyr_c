# executable('minimal', 'minimal.c', dependencies: [shyr_dep])
examples = get_option('examples')

if examples == ['all']
    executable('minimal', 'minimal.c', include_directories: includes)
else
    foreach ex : examples
        executable(ex, ex + '.c', include_directories: includes)
    endforeach
endif

