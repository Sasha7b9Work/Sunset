// 2023/08/10 19:21:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/GlobalFunctions.h"
#include "MainWindow.h"
#pragma warning(push, 0)
#include <wx/stattext.h>
#include <wx/filename.h>
#pragma warning(pop)


void GF::HorizontalAlignCenter(wxStaticText *label , wxWindow *border)
{
    wxBitmap bitmap("BITMAP_CARD");

    wxMemoryDC dc(bitmap);

    dc.SetFont(label->GetFont());

    wxSize size = dc.GetTextExtent(label->GetLabelText());

    size.x += 4;

    label->SetClientSize(size);

    wxPoint position = label->GetPosition();

    position.x = border->GetSize().x / 2 - size.x / 2;

    label->SetPosition(position);
}


wxStaticText *GF::FindStaticText(wxWindow *window, int id)
{
    return (wxStaticText *)window->FindWindow(id);
}


wxAnyButton *GF::FindAnyButton(wxWindow *window, int id)
{
    return (wxAnyButton *)window->FindWindow(id);
}


wxToggleButton *GF::FindToggleButton(wxWindow *w, int id)
{
    return (wxToggleButton *)w->FindWindow(id);
}


wxRadioButton *GF::FindRadioButton(const wxWindow *w, int id)
{
    return (wxRadioButton *)w->FindWindow(id);
}


wxCheckBox *GF::FindCheckBox(wxWindow *window, int id)
{
    return (wxCheckBox *)window->FindWindow(id);
}


ButtonsCombo *GF::FindComboBox(const wxWindow *window, int id)
{
    return (ButtonsCombo *)window->FindWindow(id);
}


wxTextCtrl *GF::FindTextCtrl(wxWindow *window, int id)
{
    return (wxTextCtrl *)window->FindWindow(id);
}


wxSpinCtrl *GF::FindSpinCtrl(wxWindow *window, int id)
{
    return (wxSpinCtrl *)window->FindWindow(id);
}


int GF::IntFromTextCtrl(wxWindow *window, int id)
{
    wxTextCtrl *control = FindTextCtrl(window, id);

    wxString value = control->GetValue();

    int result = 0;

    value.ToInt(&result);

    return result;
}


uint64 GF::UInt64FromTextCtrl(wxWindow *window, int id)
{
    wxTextCtrl *control = FindTextCtrl(window, id);

    wxString value = control->GetValue();

    unsigned long long ull_result = 0;

    value.ToULongLong(&ull_result);

    return (uint64)ull_result;
}


void GF::IntToTextCtrl(wxWindow *window, int id, int value)
{
    FindTextCtrl(window, id)->SetValue(wxString::Format("%d", value));
}


void GF::ShowModalInfo(wxWindow *windows, const wxString &text)
{
    wxMessageDialog(windows, text, "", wxCENTRE | wxICON_INFORMATION | wxOK).ShowModal();
}


void GF::SendCommandEvent(wxWindow *window, int id, int type_event, int int_value)
{
    wxCommandEvent evt(type_event, id);
    evt.SetInt(int_value);
    wxPostEvent(window->GetEventHandler(), evt);
}


void GF::SendCommandEvent(wxWindow *object, int type_event, int int_value)
{
    wxCommandEvent event(type_event, object->GetId());
    event.SetEventObject(object);
    event.SetInt(int_value);
    wxPostEvent(object->GetEventHandler(), event);
}


bool GF::IsBoardPCM()
{
    wxSize screenSize = wxGetDisplaySize(); // Получить общее разрешение экрана
    int width = screenSize.GetWidth();
    int height = screenSize.GetHeight();

    return (width == MainWindow::WIDTH) && (height == MainWindow::HEIGHT);
}


wxPoint GF::GetCoordCenter(const wxSize &size)
{
    wxPoint pos_win = { (MainWindow::WIDTH - size.x) / 2, (MainWindow::HEIGHT - size.y) / 2 };

    if (!IsBoardPCM())
    {
        pos_win += TheMainWindow->GetPosition();
    }

    return pos_win;
}


wxString GF::DirForModFiles()
{
    wxString path = wxGetCwd() + L("/mod");

    wxString normalizedPath = wxFileName::DirName(path).GetFullPath();

    if (!wxDirExists(path))
    {
        wxFileName::Mkdir(normalizedPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    return normalizedPath;
}


bool GF::ApproxEqual(double a, double b)
{
    if (std::signbit(a) != std::signbit(b))
    {
        return false;
    }

    a = std::abs(a);
    b = std::abs(b);

    if (std::abs(a - b) < std::numeric_limits<double>::epsilon())
    {
        return true;
    }

    double epsilon = 1.00002;

    if (a > b)
    {
        return a / b < epsilon;
    }
    else
    {
        return b / a < epsilon;
    }
}


wxString GF::GetSelfIP()
{
#ifdef WIN32

    // Windows implementation
    PIP_ADAPTER_ADDRESSES adapter_addresses = nullptr;
    ULONG buffer_size = 15000;
    DWORD result = 0;

    // Получаем адаптеры (может потребоваться несколько попыток)
    for (int i = 0; i < 3; ++i)
    {
        adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(buffer_size);
        if (!adapter_addresses)
        {
            return "";
        }

        result = GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, adapter_addresses, &buffer_size);
        if (result == ERROR_SUCCESS)
        {
            break;
        }
        else if (result == ERROR_BUFFER_OVERFLOW)
        {
            free(adapter_addresses);
            adapter_addresses = nullptr;
        }
        else
        {
            free(adapter_addresses);
            return "";
        }
    }

    if (result != ERROR_SUCCESS)
    {
        free(adapter_addresses);
        return "";
    }

    struct Interface
    {
        std::wstring name;
        std::string ipv4;
        std::string ipv6;
    };

    std::vector<Interface> interfaces;

    // Обходим адаптеры
    for (PIP_ADAPTER_ADDRESSES adapter = adapter_addresses; adapter != nullptr; adapter = adapter->Next)
    {
        Interface iface;
        iface.name = adapter->FriendlyName;

        // Обходим адреса адаптера
        for (PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
            address != nullptr;
            address = address->Next)
        {

            sockaddr *sa = address->Address.lpSockaddr;
            if (sa->sa_family == AF_INET)
            {
                // IPv4
                sockaddr_in *sin = (sockaddr_in *)sa;
                char ip_str[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sin->sin_addr), ip_str, INET_ADDRSTRLEN);
                iface.ipv4 = ip_str;
            }
            else if (sa->sa_family == AF_INET6)
            {
                // IPv6
                sockaddr_in6 *sin6 = (sockaddr_in6 *)sa;
                char ip_str[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, &(sin6->sin6_addr), ip_str, INET6_ADDRSTRLEN);
                iface.ipv6 = ip_str;
            }
        }

        interfaces.push_back(iface);
    }

    free(adapter_addresses);

    return wxString(interfaces[0].ipv4);

#else

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST] = { '\0' };

    if (getifaddrs(&ifaddr) == -1)
    {
        return "ERROR getifaddrs()";
    }

    wxString result;

    // Проходим по всем интерфейсам
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        // Отображаем IPv4 адреса
        if (family == AF_INET)
        {
            s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

            if (s != 0)
            {
                LOG_ERROR("getnameinfo() failed: %s", gai_strerror(s));

                continue;
            }

            if (wxString("lo") != ifa->ifa_name)
            {
                result += host;
                result += " ";
            }
        }
    }

    freeifaddrs(ifaddr);

    return result;

#endif
}
