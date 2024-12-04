#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* external
#include <json.hpp>

//* c++
#include <filesystem>

////////////////////////////////////////////////////////////////////////////////////////////
// using
////////////////////////////////////////////////////////////////////////////////////////////
using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////////////////
// JsonAdapter class
////////////////////////////////////////////////////////////////////////////////////////////
class JsonAdapter {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	//! @brief Jsonファイル読み込み
	static json LoadFromJson(const std::filesystem::path& filename);

	//! @brief Jsonファイル読み込み
	//! @retval true  成功
	//! @retval false 失敗
	static bool TryLoadFromJson(const std::filesystem::path& filename, json& out);

	//! @brief Jsonファイルに書き込み
	static void WriteToJson(const std::filesystem::path& filename, const json& data);

private:
};