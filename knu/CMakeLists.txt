include_directories(
    ${QT_INCLUDE_DIR}
    ${KDE1_INCLUDE_DIR}
)

set(KNU_SRCS CommandDlg.cpp FingerDlg.cpp HostDlg.cpp knu.cpp
    MtrDlg.cpp OptionsDlg.cpp PingDlg.cpp TracerouteDlg.cpp
)

set(KNU_MOC CommandDlg.moc FingerDlg.moc HostDlg.moc knu.moc
    MtrDlg.moc OptionsDlg.moc PingDlg.moc TracerouteDlg.moc
)

QT1_WRAP_MOC(MOC_FILES ${KNU_MOC})

add_executable(knu ${KNU_SRCS} ${MOC_FILES})
target_link_libraries(knu
    ${QT_LIBRARIES}
    ${X11_LIBRARIES}
    ${KDE1_KDECORE}
    ${KDE1_KDEUI}
    ${KDE1_KFILE}
    ${KDE1_KFM}
)

install(TARGETS knu RUNTIME DESTINATION ${KDE1_BINDIR})
install(FILES knu.kdelnk DESTINATION ${KDE1_APPSDIR}/Internet)
install_icon(knu.xpm)
