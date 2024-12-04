#pragma once

//-----------------------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------------------
//* Texture
#include "Texture.h"

//* lib
#include <Lib/Sxl/LowerUnorderedMap.h>

//* c++
#include <memory>
#include <string>
#include <filesystem>

////////////////////////////////////////////////////////////////////////////////////////////
// concept
////////////////////////////////////////////////////////////////////////////////////////////
template <class T>
concept TextureConcept = std::is_base_of_v<BaseTexture, T>;

////////////////////////////////////////////////////////////////////////////////////////////
// TextureCollection class
////////////////////////////////////////////////////////////////////////////////////////////
class TextureCollection {
public:

	//=========================================================================================
	// public methods
	//=========================================================================================

	TextureCollection()  = default;
	~TextureCollection() { Term(); }

	void Init();

	void Term();

	//* texture create methods *// //!< 関数名の見直し

	std::shared_ptr<BaseTexture> TryLoadTextureSafely(const std::filesystem::path& filename, const DirectXThreadContext* context);
	std::shared_ptr<Texture> TryLoadTexture(const std::filesystem::path& filename, const DirectXThreadContext* context);

	std::shared_ptr<BaseTexture> TryCreateRenderTextureSafely(const std::string& key, const Vector2ui& size, const Color4f& clearColor = kDefaultTextureClearColor, DXGI_FORMAT format = DxObject::kOffscreenFormat);
	std::shared_ptr<RenderTexture> TryCreateRenderTexture(const std::string& key, const Vector2ui& size, const Color4f& clearColor = kDefaultTextureClearColor, DXGI_FORMAT format = DxObject::kOffscreenFormat);

	std::shared_ptr<BaseTexture> TryCreateUnorderedTextureSafely(const std::string& key, const Vector2ui& size, DXGI_FORMAT format = DxObject::kOffscreenFormat);
	std::shared_ptr<UnorderedTexture> TryCreateUnorderedTexture(const std::string& key, const Vector2ui& size, DXGI_FORMAT format = DxObject::kOffscreenFormat);

	//* texture get methods *//

	template <TextureConcept T = BaseTexture>
	std::shared_ptr<T> GetTexture(const std::string& key) const;

	const D3D12_GPU_DESCRIPTOR_HANDLE& GetGPUHandleSRV(const std::string& key) const;

	//* texture release *//

	void ReleaseTexture(const std::string& key);

private:

	//=========================================================================================
	// private variables
	//=========================================================================================

	Sxl::LowerUnorderedMapA<std::shared_ptr<BaseTexture>> textures_;
	//!< 自作したtextureまでもがlower map に入るのはおかしい...?

	//=========================================================================================
	// private methods
	//=========================================================================================

	template <TextureConcept T>
	std::shared_ptr<T> GetTexture(const std::shared_ptr<BaseTexture>& base) const;
	
};

////////////////////////////////////////////////////////////////////////////////////////////
// TextureCollection class template methods
////////////////////////////////////////////////////////////////////////////////////////////

template<TextureConcept T>
inline std::shared_ptr<T> TextureCollection::GetTexture(const std::string& key) const {
	Assert(textures_.Contains(key), "Texture is not found.");
	return GetTexture<T>(textures_.At(key));
}

template <TextureConcept T>
inline std::shared_ptr<T> TextureCollection::GetTexture(const std::shared_ptr<BaseTexture>& base) const {
	std::shared_ptr<T> texture = std::dynamic_pointer_cast<T>(base);
	Assert(texture != nullptr, "Texture don't match concept.");
	return texture;
}
