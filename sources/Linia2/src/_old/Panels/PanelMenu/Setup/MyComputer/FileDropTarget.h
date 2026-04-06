#pragma once
#include "FilePanel.h"


class FileDropTarget : public wxFileDropTarget {
public:
    FileDropTarget(FilePanel *panel);
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override;

private:
    FilePanel *m_panel;
};
