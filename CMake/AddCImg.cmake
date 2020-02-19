if(NOT EXISTS ${CMAKE_SOURCE_DIR}/include/CImg)
    message(STATUS "File CImg.h not found, trying to download...")

    set(CIMG_URL https://raw.githubusercontent.com/dtschump/CImg/master/CImg.h)

    if(APPLE)
        execute_process(COMMAND curl --output ${CMAKE_SOURCE_DIR}/include/CImg/CImg.h ${CIMG_URL}
            TIMEOUT 3
        )
    elseif(UNIX)
        execute_process(COMMAND wget --output-document=${CMAKE_SOURCE_DIR}/include/CImg/CImg.h ${CIMG_URL}
            TIMEOUT 3
        )
    else()
        message(FATAL_ERROR "Cannot download source file CImg.h, try download it manually from "${CIMG_URL}"and place it under "${CMAKE_SOURCE_DIR}"/include/CImg")
    endif()
    

    set(CImg_EXISTS TRUE)
endif()