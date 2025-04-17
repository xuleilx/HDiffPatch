cmake_minimum_required(VERSION 3.5)

# 定义项目名称
project(hpatchz)

# 定义变量
set(HDP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../")

# 默认编译选项
set(DEF_FLAGS "-DNDEBUG -D_LARGEFILE_SOURCE -D_IS_NEED_CACHE_OLD_BY_COVERS=0 -D_IS_NEED_DEFAULT_CompressPlugin=0")

# 默认链接选项
# set(LINK_FLAGS "-llog")

# 源文件列表
set(Src_Files
    "${HDP_PATH}/builds/cmake/hpatch.c"
    "${HDP_PATH}/file_for_patch.c"
    "${HDP_PATH}/libHDiffPatch/HPatch/patch.c"
)

# 条件编译选项和源文件
option(ZLIB "Enable ZLIB support" ON)
option(LZMA "Enable LZMA support" OFF)
option(ZSTD "Enable ZSTD support" OFF)
option(BROTLI "Enable BROTLI support" OFF)
option(VCD "Enable VCD support" OFF)
# if open BSD,must open BZIP2
option(BSD "Enable BSD support" OFF)
option(BZIP2 "Enable BZIP2 support" ON)

if(BSD)
    list(APPEND DEF_FLAGS "-D_IS_NEED_BSDIFF=1")
    list(APPEND Src_Files "${HDP_PATH}/bsdiff_wrapper/bspatch_wrapper.c")
else()
    list(APPEND DEF_FLAGS "-D_IS_NEED_BSDIFF=0")
endif()

if(VCD)
    list(APPEND DEF_FLAGS "-D_IS_NEED_VCDIFF=1")
    list(APPEND Src_Files
        "${HDP_PATH}/vcdiff_wrapper/vcpatch_wrapper.c"
        "${HDP_PATH}/libHDiffPatch/HDiff/private_diff/limit_mem_diff/adler_roll.c"
    )
else()
    list(APPEND DEF_FLAGS "-D_IS_NEED_VCDIFF=0")
endif()

if(ZLIB)
    list(APPEND DEF_FLAGS "-D_CompressPlugin_zlib")
    list(APPEND LINK_FLAGS "-lz")
endif()

if(BZIP2)
# http://www.bzip.org  https://github.com/sisong/bzip2
    list(APPEND DEF_FLAGS "-D_CompressPlugin_bz2 -DBZ_NO_STDIO")
    include_directories("${HDP_PATH}/../bzip2/")
    list(APPEND Src_Files
        "${HDP_PATH}/../bzip2/blocksort.c"
        "${HDP_PATH}/../bzip2/bzlib.c"
        "${HDP_PATH}/../bzip2/compress.c"
        "${HDP_PATH}/../bzip2/crctable.c"
        "${HDP_PATH}/../bzip2/decompress.c"
        "${HDP_PATH}/../bzip2/huffman.c"
        "${HDP_PATH}/../bzip2/randtable.c"
    )
endif()

if(LZMA)
# https://github.com/sisong/lzma
    list(APPEND DEF_FLAGS "-D_CompressPlugin_lzma -D_CompressPlugin_lzma2 -DZ7_ST")
    include_directories("${HDP_PATH}/../lzma/C/")
    list(APPEND Src_Files
        "${HDP_PATH}/../lzma/C/LzmaDec.c"
        "${HDP_PATH}/../lzma/C/Lzma2Dec.c"
    )
    if(TARGET_ARCH_ABI STREQUAL "arm64-v8a")
        list(APPEND DEF_FLAGS "-DZ7_LZMA_DEC_OPT")
        list(APPEND Src_Files "${HDP_PATH}/../lzma/Asm/arm64/LzmaDecOpt.S")
    endif()
    if(VCD)
        list(APPEND DEF_FLAGS "-D_CompressPlugin_7zXZ")
        list(APPEND Src_Files
            "${HDP_PATH}/../lzma/C/7zCrc.c"
            "${HDP_PATH}/../lzma/C/7zCrcOpt.c"
            "${HDP_PATH}/../lzma/C/Bra.c"
            "${HDP_PATH}/../lzma/C/Bra86.c"
            "${HDP_PATH}/../lzma/C/BraIA64.c"
            "${HDP_PATH}/../lzma/C/Delta.c"
            "${HDP_PATH}/../lzma/C/Sha256.c"
            "${HDP_PATH}/../lzma/C/Sha256Opt.c"
            "${HDP_PATH}/../lzma/C/Xz.c"
            "${HDP_PATH}/../lzma/C/XzCrc64.c"
            "${HDP_PATH}/../lzma/C/XzCrc64Opt.c"
            "${HDP_PATH}/../lzma/C/XzDec.c"
            "${HDP_PATH}/../lzma/C/CpuArch.c"
        )
    endif()
endif()

if(ZSTD)
# https://github.com/sisong/zstd
    list(APPEND DEF_FLAGS "-D_CompressPlugin_zstd -DZSTD_HAVE_WEAK_SYMBOLS=0 -DZSTD_TRACE=0 -DZSTD_DISABLE_ASM=1 -DZSTDLIB_VISIBLE= -DZSTDLIB_HIDDEN= \
		  -DDYNAMIC_BMI2=0 -DZSTD_LEGACY_SUPPORT=0 -DZSTD_LIB_DEPRECATED=0 -DHUF_FORCE_DECOMPRESS_X1=1 \
		  -DZSTD_FORCE_DECOMPRESS_SEQUENCES_SHORT=1 -DZSTD_NO_INLINE=1 -DZSTD_STRIP_ERROR_STRINGS=1 -DZSTDERRORLIB_VISIBILITY")
    include_directories("${HDP_PATH}/../zstd/lib/" "${HDP_PATH}/../zstd/lib/common" "${HDP_PATH}/../zstd/lib/decompress")
    list(APPEND Src_Files
        "${HDP_PATH}/../zstd/lib/common/debug.c"
        "${HDP_PATH}/../zstd/lib/common/entropy_common.c"
        "${HDP_PATH}/../zstd/lib/common/error_private.c"
        "${HDP_PATH}/../zstd/lib/common/fse_decompress.c"
        "${HDP_PATH}/../zstd/lib/common/xxhash.c"
        "${HDP_PATH}/../zstd/lib/common/zstd_common.c"
        "${HDP_PATH}/../zstd/lib/decompress/huf_decompress.c"
        "${HDP_PATH}/../zstd/lib/decompress/zstd_ddict.c"
        "${HDP_PATH}/../zstd/lib/decompress/zstd_decompress.c"
        "${HDP_PATH}/../zstd/lib/decompress/zstd_decompress_block.c"
    )
endif()

if(BROTLI)
# https://github.com/google/brotli
    list(APPEND DEF_FLAGS "-D_CompressPlugin_brotli")
    include_directories("${HDP_PATH}/../brotli/c/include")
    list(APPEND Src_Files
        "${HDP_PATH}/../brotli/c/common/constants.c"
        "${HDP_PATH}/../brotli/c/common/context.c"
        "${HDP_PATH}/../brotli/c/common/dictionary.c"
        "${HDP_PATH}/../brotli/c/common/platform.c"
        "${HDP_PATH}/../brotli/c/common/shared_dictionary.c"
        "${HDP_PATH}/../brotli/c/common/transform.c"
        "${HDP_PATH}/../brotli/c/dec/bit_reader.c"
        "${HDP_PATH}/../brotli/c/dec/decode.c"
        "${HDP_PATH}/../brotli/c/dec/huffman.c"
        "${HDP_PATH}/../brotli/c/dec/state.c"
    )
endif()

# # 添加共享库目标
# add_library(hpatchz SHARED ${Src_Files})

# 添加静态库目标
add_library(hpatchz STATIC ${Src_Files})

# 设置编译选项和链接选项
target_compile_options(hpatchz PRIVATE ${DEF_FLAGS})
target_link_libraries(hpatchz PRIVATE ${LINK_FLAGS})

# 添加可执行文件 myhpatchz
add_executable(myhpatchz "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# 链接共享库 hpatchz
target_link_libraries(myhpatchz PRIVATE hpatchz)

# 设置编译选项（如果需要）
target_compile_options(myhpatchz PRIVATE ${DEF_FLAGS})