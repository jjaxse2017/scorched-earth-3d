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

#if !defined(AFX_WeaponDelay_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
#define AFX_WeaponDelay_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_

#include <actions/CallbackWeapon.h>

class WeaponDelay  : public WeaponCallback
{
public:
	WeaponDelay();
	virtual ~WeaponDelay();

	// Inherited from Weapon
	virtual bool parseXML(AccessoryCreateContext &context,
		XMLNode *accessoryNode);
	void fireWeapon(ScorchedContext &context,
		WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity);

	REGISTER_ACCESSORY_HEADER(WeaponDelay, AccessoryPart::AccessoryWeapon);

	// Inhirited from CallbackWeapon
	virtual void weaponCallback(
			ScorchedContext &context,
			WeaponFireContext &weaponContext, FixedVector &position, FixedVector &velocity,
			unsigned int userData);

protected:
	NumberParser delay_;
	Weapon *delayedWeapon_;

};

#endif // !defined(AFX_WeaponDelay_H__B5C043F0_7DC6_4198_AE5B_E19002234FCE__INCLUDED_)
