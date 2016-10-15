include_directories(
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(KORN_SRCS kmailbox.cpp kornbutt.cpp korn.cpp kornopt.cpp kornset.cpp
    main.cpp optdata.cpp real_from.cpp
)

set(KORN_MOC kmailbox.moc kornbutt.moc korn.moc kornopt.moc optdata.moc)

QT1_WRAP_MOC(MOC_FILES ${KORN_MOC})

add_executable(korn ${KORN_SRCS} ${MOC_FILES})
target_link_libraries(korn
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
)

install(TARGETS korn RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES KOrn.kdelnk DESTINATION ${KDE1_DATADIR}/korn)
install_icon(korn.xpm)


