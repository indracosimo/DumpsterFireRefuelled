#pragma once
struct Resolution 
{
	int width;
	int height;
};

namespace config 
{
	static constexpr Resolution SD = {1280, 720};
	static constexpr Resolution HD = {1920, 1080};
	static constexpr Resolution QHD = {2560, 1440};
	static constexpr Resolution UHD4K = {3840, 2169};
}