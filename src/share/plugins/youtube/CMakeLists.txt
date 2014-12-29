add_subdirectory(icons)
find_package(KF5Wallet)

add_share_plugin(youtubeplugin youtubemanager.cpp kipiplugin_youtube.cpp youtubejob.cpp youtubejobcomposite.cpp)
target_link_libraries(youtubeplugin KF5::WidgetsAddons KF5::KIOCore KF5::I18n KF5::Wallet)

