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

#if !defined(__INCLUDE_SaveSelectDialogh_INCLUDE__)
#define __INCLUDE_SaveSelectDialogh_INCLUDE__

#include <GLW/GLWWindow.h>
#include <GLW/GLWButton.h>
#include <GLW/GLWIconList.h>
#include <GLW/GLWIcon.h>
#include <common/ToolTip.h>

class GLWIconListSaveItem : public GLWIconListItem
{
public:
	GLWIconListSaveItem(const char *file, const char *time);
	virtual ~GLWIconListSaveItem();

	const char *getFile() { return file_.c_str(); }

	// GLWIconListItem
	virtual void draw(float x, float y, float w);

protected:
	GLWIcon icon_;
	ToolTip tip_;
	std::string file_;
	std::string time_;
};

class SaveSelectDialog : 
	public GLWWindow,
	public GLWButtonI, 
	public GLWIconListI
{
public:
	static SaveSelectDialog *instance();

	// GLWWindow
	virtual void display();

	// GLWButtonI
	virtual void buttonDown(unsigned int id);

	// GLWIconListI
	virtual void selected(unsigned int id, int position);
	virtual void chosen(unsigned int id, int position);

protected:
	static SaveSelectDialog *instance_;

	GLWButton *ok_;
	unsigned int cancelId_;
	GLWIconList *iconList_;

private:
	SaveSelectDialog();
	virtual ~SaveSelectDialog();

};

#endif
