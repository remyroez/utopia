
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace {

struct AppState {
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	Uint64 last_step = 0;
};

constexpr const char *initial_window_title = "utopia";

constexpr int initial_window_width = 640;

constexpr int initial_window_height = 480;

constexpr SDL_WindowFlags initial_window_flags = SDL_WINDOW_RESIZABLE;

constexpr Uint64 step_rate_in_milliseconds = 1000 / 60;

template <class T, size_t N = 1>
T *alloc() {
	return static_cast<T*>(SDL_calloc(N, sizeof(T)));
}

} // namespace

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
	SDL_AppResult result = SDL_APP_FAILURE;

	if (!SDL_SetAppMetadata("utopia", "0.1", "com.remyroez.Utopia")) {
		// メタデータ設定失敗

	} else if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK)) {
		// 初期化失敗

	} else if (auto *state = static_cast<AppState*>(*appstate = ::alloc<AppState>()); state == nullptr) {
		// メモリ確保失敗

	} else if (!SDL_CreateWindowAndRenderer(
		::initial_window_title,
		::initial_window_width,
		::initial_window_height,
		::initial_window_flags,
		&state->window,
		&state->renderer)) {
		// ウィンドウとレンダラーの生成失敗

	} else {
		SDL_SetWindowMinimumSize(state->window, 128 * 2, 204);
		state->last_step = SDL_GetTicks();
		result = SDL_APP_CONTINUE;
	}

	return result;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result)
{
	if (appstate != NULL) {
		auto *state = static_cast<AppState*>(appstate);
		SDL_DestroyRenderer(state->renderer);
		SDL_DestroyWindow(state->window);
		SDL_free(state);
	}
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
	SDL_AppResult result = SDL_APP_CONTINUE;

	[[maybe_unused]] auto *state = static_cast<AppState *>(appstate);

	switch (event->type) {
	case SDL_EVENT_QUIT:
		result = SDL_APP_SUCCESS;
		break;
	default:
		break;
	}

	return result;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
	auto *state = static_cast<AppState*>(appstate);
	const Uint64 now = SDL_GetTicks();

	int count = 0;
	static float x = 0.f;
	while ((now - state->last_step) >= step_rate_in_milliseconds) {
		x += 1.f;
		state->last_step += step_rate_in_milliseconds;
		count++;
	}
	if (x > 640.f) x -= 640.f;

	SDL_SetRenderDrawColor(state->renderer, 0x80, 0x80, 0x80, 0x80);
	SDL_RenderClear(state->renderer);

	{
		SDL_FRect rect{ .x = x, .y = 100, .w = 32, .h = 32 };
		SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0xFF);
		SDL_RenderFillRect(state->renderer, &rect);
	}

	{
		SDL_FRect rect{ .x = 0, .y = 0, .w = static_cast<float>(count * 32), .h = 32 };
		SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 0xFF);
		SDL_RenderFillRect(state->renderer, &rect);
	}

	SDL_RenderPresent(state->renderer);

	return SDL_APP_CONTINUE;
}
