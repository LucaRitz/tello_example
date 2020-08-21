include(FetchContent)
FetchContent_Declare(
        tello
        GIT_REPOSITORY https://github.com/LucaRitz/tello.git
        GIT_TAG        origin/master
)
FetchContent_MakeAvailable(tello)
