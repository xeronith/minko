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

#include "minko/Common.hpp"

#include "minko/component/AbstractComponent.hpp"

namespace minko
{
	namespace component
	{
		class Surface :
			public AbstractComponent,
            public Uuid::enable_uuid
		{
			friend render::DrawCallPool;

		public:
			typedef std::shared_ptr<Surface>								Ptr;
			typedef Signal<Ptr, const std::string&, bool>					TechniqueChangedSignal;	
			typedef Signal<Ptr, std::shared_ptr<component::Renderer>, bool> VisibilityChangedSignal;

		private:
			typedef std::shared_ptr<scene::Node>    NodePtr;
			typedef std::shared_ptr<render::Effect>	EffectPtr;
			typedef const std::string&				StringRef;

        public:
            static const std::string SURFACE_COLLECTION_NAME;
            static const std::string GEOMETRY_COLLECTION_NAME;
            static const std::string MATERIAL_COLLECTION_NAME;
            static const std::string EFFECT_COLLECTION_NAME;

		private:
			std::string								_name;

			std::shared_ptr<geometry::Geometry>		_geometry;
			std::shared_ptr<material::Material>		_material;
			std::shared_ptr<render::Effect>			_effect;
			std::string 							_technique;
            std::shared_ptr<data::Provider>         _provider;

            Signal<Ptr>                             _geometryChanged;
            Signal<Ptr>                             _materialChanged;
            Signal<Ptr>                             _effectChanged;

		public:
			static
			Ptr
			create(std::shared_ptr<geometry::Geometry> 		geometry,
				   std::shared_ptr<material::Material>		material,
				   std::shared_ptr<render::Effect>			effect)
			{
				return create("", geometry, material, effect, "default");
			}

			static
			Ptr
			create(const std::string&					    name,
				   std::shared_ptr<geometry::Geometry> 		geometry,
				   std::shared_ptr<material::Material>      material,
				   std::shared_ptr<render::Effect>			effect,
				   const std::string&						technique)
			{
                return std::shared_ptr<Surface>(new Surface(name, geometry, material, effect, technique));
			}

			~Surface()
			{
			}

			inline
			const std::string&
			name() const
			{
				return _name;
			}

			inline
			void
			name(const std::string& value)
			{
				_name = value;
			}

			inline
			std::shared_ptr<geometry::Geometry>
			geometry() const
			{
				return _geometry;
			}

            void
            geometry(std::shared_ptr<geometry::Geometry> geometry);

			inline
			std::shared_ptr<material::Material>
			material() const
			{
				return _material;
			}

            void
            material(std::shared_ptr<material::Material> material);

			inline
			std::shared_ptr<render::Effect>
			effect() const
			{
				return _effect;
			}

			inline
			const std::string&
			technique() const
			{
				return _technique;
			}

			void
			effect(std::shared_ptr<render::Effect>, const std::string& = "default");

            inline
            Signal<Ptr>&
            geometryChanged()
            {
                return _geometryChanged;
            }

            inline
            Signal<Ptr>&
            materialChanged()
            {
                return _materialChanged;
            }

            inline
            Signal<Ptr>&
            effectChanged()
            {
                return _effectChanged;
            }

        protected:
			void
			targetAdded(NodePtr);

			void
			targetRemoved(NodePtr);

		private:
			Surface(std::string								name,
					std::shared_ptr<geometry::Geometry>		geometry,
					std::shared_ptr<material::Material>	    material,
					std::shared_ptr<render::Effect>			effect,
					const std::string&						technique);

			void
			setEffectAndTechnique(EffectPtr, const std::string&);
		};
	}
}