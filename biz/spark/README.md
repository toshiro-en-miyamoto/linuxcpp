# spark
SPark BMP to TTF converter

Module depencency

b2t
    main()
        is_bmp_path_valid()
        is_ttf_path_valid()    
        bmp_files_in()
        ttedit_convert()

b2tedit
    ttedit_convert(std::vector<Bmp_file>&, fs::path&)

b2tutil
    struct Bmp_file
    bmp_files_in(const fs::path&)
        bmp_file_vector()
        codepoint_of_bmp_file()
    is_bmp_path_valid(const fs::path&)
    is_ttf_path_valid(const fs::path&)

b2tutil_impl
    bmp_file_vector()
    codepoint_of_bmp_filename(const fs::path&)
