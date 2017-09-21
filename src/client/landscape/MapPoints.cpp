////////////////////////////////////////////////////////////////////////////////
//    Scorched3D (c) 2000-2011
//
//    This file is part of Scorched3D.
//
//    Scorched3D is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    Scorched3D is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
////////////////////////////////////////////////////////////////////////////////

#include <landscape/MapPoints.h>
#include <3dsparse/ModelStore.h>
#include <graph/ModelRenderer.h>
#include <graph/ModelRendererSimulator.h>
#include <graph/ModelRendererStore.h>

MapPoints *MapPoints::instance_ = 0;

MapPoints *MapPoints::instance()
{
	if (!instance_) instance_ = new MapPoints;
	return instance_;
}

MapPoints::MapPoints() 
{
	{
		ModelID id;
		id.initFromString("ase", "data/meshes/wrap.ase", "none");
		borderModelWrap_ = new ModelRendererSimulator(
			ModelRendererStore::instance()->loadModel(id));
	}
	{
		ModelID id;
		id.initFromString("ase", "data/meshes/bounce.ase", "none");
		borderModelBounce_ =  new ModelRendererSimulator(
			ModelRendererStore::instance()->loadModel(id));
	}
	{
		ModelID id;
		id.initFromString("ase", "data/meshes/concrete.ase", "none");
		borderModelConcrete_ = new ModelRendererSimulator(
			ModelRendererStore::instance()->loadModel(id));
	}
}

MapPoints::~MapPoints()
{
	delete borderModelWrap_;
	delete borderModelBounce_;
	delete borderModelConcrete_;
}

