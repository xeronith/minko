/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "minko/SerializerCommon.hpp"

namespace minko
{
	namespace deserialize
	{
		class ComponentDeserializer
		{

		private:
			typedef std::shared_ptr<math::Matrix4x4>				Matrix4x4Ptr;
			typedef std::shared_ptr<file::Dependency>				DependencyPtr;
			typedef std::shared_ptr<file::AssetLibrary>				AssetLibraryPtr;
			typedef std::shared_ptr<component::AbstractComponent>	AbsComponentPtr;

		public:
			static
			AbsComponentPtr
			deserializeTransform(std::string&		serializedTransformData,
								 AssetLibraryPtr	assetLibrary,
								 DependencyPtr		dependencies);

			static
			AbsComponentPtr
			deserializeProjectionCamera(std::string&	serializedCameraData,
										AssetLibraryPtr	assetLibrary,
										DependencyPtr	dependencies);

			static
			AbsComponentPtr
			deserializeAmbientLight(std::string&	serializedAmbientLight,
									AssetLibraryPtr	assetLibrary,
									DependencyPtr	dependencies);

			static
			AbsComponentPtr
			deserializeDirectionalLight(std::string&	serializedDirectionalLight,
										AssetLibraryPtr	assetLibrary,
										DependencyPtr	dependencies);

			static
			AbsComponentPtr
			deserializePointLight(std::string&		serializedPointLight,
								  AssetLibraryPtr	assetLibrary,
								  DependencyPtr		dependencies);
		
			static
			AbsComponentPtr
			deserializeSpotLight(std::string&		serializedSpotLight,
								 AssetLibraryPtr	assetLibrary,
								 DependencyPtr		dependencies);

			static
			AbsComponentPtr
			deserializeSurface(std::string&		serializedSurface,
							   AssetLibraryPtr	assetLibrary,
							   DependencyPtr	dependencies);

			static
			AbsComponentPtr
			deserializeRenderer(std::string&	serializedRenderer,
							   AssetLibraryPtr	assetLibrary,
							   DependencyPtr	dependencies);
		};
	}
}
