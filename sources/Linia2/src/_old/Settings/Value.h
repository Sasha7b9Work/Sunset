// 2024/02/03 12:50:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Configurator.h"
#include "Utils/GlobalFunctions.h"
#include "Controls/Buttons.h"


template<class T>
class Value
{
public:
    Value(const wxString &_key, const T &_default_value, bool _serialiazed = true) : //-V730
        key(_key),
        default_value(_default_value),
        stored_value(_default_value),
        serialiazed(_serialiazed)
    {}

    T Get() const
    {
        return value;
    }

    T GetValueAndReset()
    {
        T result = value;

        Reset();

        return result;
    }

    virtual void Set(const T &_value)
    {
        value = _value;

        if (serialiazed)
        {
            Config::Write(key, value);
            Config::Flush();
        }
    }

    virtual void Load()
    {
        value = Config::Read(key, default_value);
    }

    virtual void Save()
    {
        Set(value);
    }

    virtual void ResetToFactory(bool factory)
    {
        if (factory)
        {
            stored_value = GetValueAndReset();
        }
        else
        {
            Set(stored_value);
        }
    }

    // Установить значение переменной из контрола
    virtual void SetFromControl() { };

    bool ValueIsDefault() const
    {
        return Get() == GetDefault();
    }

    void Reset()
    {
        Set(default_value);
    }

private:

    T GetDefault() const
    {
        return default_value;
    }

    wxString GetKey() const
    {
        return key;
    }

protected:
    wxString key;
    T        default_value;
    T        stored_value;      // Сюда записываем значение перед тем как загрузить заводскую настроку,
                                // чтобы потом восстановить

    void Init(const wxString &_key, const T &_default_value)
    {
        key = _key;
        default_value = _default_value;

        Load();
    }

    // Загрузить значение непосредственно в элемент управления
    virtual void LoadToWindow() { };

private:
    T value;
    const bool serialiazed;
};


struct ValueBool : public Value<bool>
{
    ValueBool(const wxString &_key, const bool &_def);
};

struct ValueCheckBox : public wxCheckBox
{
    ValueCheckBox(wxWindow *parent, const wxString &title, const wxPoint &position, const wxString &_key, const bool &_def);

    void SetNewValue(bool);

private:

    ValueBool *value = nullptr;

    // Закрыли, чтобы случайно не вызвать
    virtual void SetValue(bool value) override;
};


struct ValueInt : public Value<int>
{
    ValueInt(const wxString &_key, const int &_def);
};


struct ValueUInt : public Value<uint>
{
    ValueUInt(const wxString &_key, const uint &_def);
};


struct ValuePoint : public Value<wxPoint>
{
    ValuePoint(const wxString &_key, const wxPoint &_def);
};
