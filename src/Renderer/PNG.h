#ifndef PNG_h_
#define PNG_h_

#include <stdexcept>
#include <png.h>
#include "ResourceManager/ResourceManager.h"
#include "Image.h"

class PNG : public Image, public Resource
{
public:
	PNG(std::shared_ptr<ResourceBundle::Block> block);
	std::size_t Size() override;
	~PNG();

private:
	static void pngErrorFunction(png_structp png_ptr, png_const_charp error_msg);
	static void pngWarningFunction(png_structp png_ptr, png_const_charp warning_msg);
	static void pngReadFunction(png_structp png_ptr, png_bytep data, png_size_t length);
};

#endif
