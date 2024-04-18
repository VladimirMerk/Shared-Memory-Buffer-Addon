#include <napi.h>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>

using namespace boost::interprocess;

// Функция для записи данных в разделяемую память
void SetBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2 || !info[0].IsString() || !info[1].IsBuffer()) {
        Napi::TypeError::New(env, "String and Buffer expected").ThrowAsJavaScriptException();
        return;
    }

    std::string shared_memory_name = info[0].As<Napi::String>().Utf8Value();
    Napi::Buffer<uint8_t> buffer = info[1].As<Napi::Buffer<uint8_t>>();

    try {
        // Открываем или создаем разделяемую память
        shared_memory_object shm(open_or_create, shared_memory_name.c_str(), read_write);

        // Устанавливаем размер разделяемой памяти равным размеру данных + 3 байта для размера буфера
        std::size_t shm_size = buffer.Length() + 3;
        shm.truncate(shm_size);

        // Создаем отображение разделяемой памяти
        mapped_region region(shm, read_write);

        // Получаем указатель на начало разделяемой памяти
        void* addr = region.get_address();

        // Записываем размер буфера в первые три байта разделяемой памяти
        std::uint32_t dataSize = buffer.Length();
        std::memcpy(addr, &dataSize, sizeof(std::uint32_t));

        // Копируем данные из буфера в разделяемую память, начиная с четвертого байта
        std::memcpy(static_cast<char*>(addr) + sizeof(std::uint32_t), buffer.Data(), buffer.Length());

        // Выводим информацию в консоль
        Napi::Object console = env.Global().Get("console").As<Napi::Object>();
        // console.Get("log").As<Napi::Function>().Call(console, { Napi::String::New(env, "Data written to shared memory for key " + shared_memory_name) });
    } catch (const boost::interprocess::interprocess_exception& ex) {
        Napi::Error::New(env, ex.what()).ThrowAsJavaScriptException();
    }
}

Napi::Value GetBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    std::string shared_memory_name = info[0].As<Napi::String>().Utf8Value();

    try {
        // Открываем разделяемую память
        shared_memory_object shm(open_only, shared_memory_name.c_str(), read_only);

        // Создаем отображение разделяемой памяти
        mapped_region region(shm, read_only);

        // Получаем указатель на начало разделяемой памяти
        void* addr = region.get_address();

        // Получаем размер данных из первых трех байт разделяемой памяти
        std::uint32_t dataSize;
        std::memcpy(&dataSize, addr, sizeof(std::uint32_t));

        // Создаем буфер только с фактическим размером данных
        Napi::Buffer<uint8_t> buffer = Napi::Buffer<uint8_t>::New(env, dataSize);

        // Копируем данные из разделяемой памяти, начиная с четвертого байта
        std::memcpy(buffer.Data(), static_cast<char*>(addr) + sizeof(std::uint32_t), dataSize);

        return buffer;
    } catch (const boost::interprocess::interprocess_exception& ex) {
        Napi::Error::New(env, ex.what()).ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

// Функция для очистки разделяемой памяти
void ClearBuffer(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return; // Здесь нет необходимости возвращать значение, так как функция типа void
    }

    std::string shared_memory_name = info[0].As<Napi::String>().Utf8Value();

    try {
        // Удаляем разделяемую память по имени
        shared_memory_object::remove(shared_memory_name.c_str());
    } catch (const boost::interprocess::interprocess_exception& ex) {
        Napi::Error::New(env, ex.what()).ThrowAsJavaScriptException();
    }
}

// Инициализация модуля
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "setBuffer"), Napi::Function::New(env, SetBuffer));
    exports.Set(Napi::String::New(env, "getBuffer"), Napi::Function::New(env, GetBuffer));
    exports.Set(Napi::String::New(env, "clearBuffer"), Napi::Function::New(env, ClearBuffer));
    return exports;
}

NODE_API_MODULE(myAddon, Init)
