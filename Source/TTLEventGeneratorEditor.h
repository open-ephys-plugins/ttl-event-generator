/*
------------------------------------------------------------------
This file is part of the Open Ephys GUI
Copyright (C) 2021 Open Ephys
------------------------------------------------------------------
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TTLEventGenerator_EDITOR_H_DEFINED
#define TTLEventGenerator_EDITOR_H_DEFINED

#include <EditorHeaders.h>
#include "TTLEventGenerator.h"

class TTLEventGenerator;

class TTLEventGeneratorEditor : public GenericEditor
{
public:

	TTLEventGeneratorEditor(TTLEventGenerator* parentNode, bool useDefaultParameterEditors);
	~TTLEventGeneratorEditor();

	void updateSettings() override;

private:

	TTLEventGenerator* processor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TTLEventGeneratorEditor);

};

#endif // TTLEventGenerator_EDITOR_H