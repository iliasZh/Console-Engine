#include "console.hpp"
#include "exception.hpp"

#include <exception>

#include <fmt/format.h>
#include <yaml-cpp/yaml.h>

int main()
{
	try {
		Console console{};
		console.set_cursor({ Cursor::Preset::Invisible });
		console.set_font({ 10 });

		auto config = YAML::LoadFile("../text/title.yaml");
		fmt::print("title: {}\n", config["title"].as<std::string>());
	}
	catch (Exception& e) {
		MessageBoxA(nullptr, e.what().c_str(), "Application error",
					MB_OK | MB_ICONERROR);
	}
	catch (std::exception& e) {
		MessageBoxA(nullptr, e.what(), "C++ error", MB_OK | MB_ICONERROR);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details available", "Unknown error",
					MB_OK | MB_ICONERROR);
	}
	return 0;
}
