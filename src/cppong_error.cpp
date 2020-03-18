
int SDL_ErrorCheck(int code) {
    if(code < 0) {
        printf("SDL error: %s\n", SDL_GetError());
        abort();
    }

    return code;
}

template<typename T>
T *SDL_ErrorCheck(T *ptr) {
    if(ptr == nullptr) {
        printf("SDL error: %s\n", SDL_GetError());
        abort();
    }

    return ptr;
}

int TTF_ErrorCheck(int code) {
    if(code != 0) {
        printf("SDL TTF error: %s\n", TTF_GetError());
        abort();
    }

    return code;
}

template<typename T>
T *TTF_ErrorCheck(T *ptr) {
    if(ptr == nullptr) {
        printf("SDL TTF error: %s\n", TTF_GetError());
        abort();
    }

    return ptr;
}
