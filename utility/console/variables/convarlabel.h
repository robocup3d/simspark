/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef _CONVARLABEL_H_
#define _CONVARLABEL_H_

#include "convarstate.h"

/** $Id: convarlabel.h,v 1.1 2002/08/14 09:24:53 fruit Exp $
  * 
  *   ConVarLabel
  * 
  *      This variable is only used by the ConsoleParser and represents a label returned by the scanner.
  *      It contains a string.
  *
  *      It is not convertible to anything, its accessors are getLabel and setLabel.
  *
  *   HISTORY:
  *       15.04.02 - AF
  *         - First working version
  * 
  *   TODO:
  * 
  *   TOFIX:
  */

class ConVarLabel : public ConVarState
{
public:
   ConVarLabel (const ConVarAttributes& attributes, const std::string& value);
   
   virtual ConVarState* clone() const;
   
   virtual ConVar::EConVarType getType() const;
   
   virtual bool setLabel (const std::string& value);

   virtual bool getLabel (std::string& value) const;

   virtual std::string dumpValue() const;
   virtual std::string dumpType() const;

protected:
   virtual void setValue (std::string value);
   virtual std::string getValue() const;

private:
   std::string mValue;
};


#endif // _CONVARLABEL_H_

