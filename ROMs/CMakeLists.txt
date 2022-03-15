
INCLUDE(DownloadAndExtract)

###
### GameBoy
###

# https://gbdev.gg8.se/files/roms/bootroms/
FILE(DOWNLOAD
    "https://gbdev.gg8.se/files/roms/bootroms/dmg_boot.bin"
    ${CMAKE_CURRENT_SOURCE_DIR}/dmg_boot.bin
)

FILE(DOWNLOAD
    "https://gbdev.gg8.se/files/roms/bootroms/sgb_boot.bin"
    ${CMAKE_CURRENT_SOURCE_DIR}/sgb_boot.bin
)

FILE(DOWNLOAD
    "https://gbdev.gg8.se/files/roms/bootroms/cgb_boot.bin"
    ${CMAKE_CURRENT_SOURCE_DIR}/cgb_boot.bin
)

# https://github.com/Gekkio/mooneye-test-suite/
DOWNLOAD_AND_EXTRACT(
    "https://gekkio.fi/files/mooneye-test-suite/mts-20211031-2031-86d1acf/mts-20211031-2031-86d1acf.tar.gz"
    ${CMAKE_CURRENT_SOURCE_DIR}/mooneye
)

DOWNLOAD_AND_EXTRACT(
    "https://github.com/retrio/gb-test-roms/archive/refs/heads/master.zip"
    ${CMAKE_CURRENT_SOURCE_DIR}/blargg
)

###
###
###