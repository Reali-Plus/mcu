set(srcs "main.cpp" "icm20948.cpp"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
