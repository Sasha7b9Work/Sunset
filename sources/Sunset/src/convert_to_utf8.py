#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from pathlib import Path

def is_windows_1251(file_path):
    """
    Проверяет, является ли файл Windows-1251 (а не UTF-8 или другой кодировкой)
    """
    try:
        with open(file_path, 'rb') as f:
            content = f.read()
        
        if not content:
            return False
        
        # Проверяем наличие BOM (UTF-8 with BOM)
        if content.startswith(b'\xef\xbb\xbf'):
            return False
        
        # Пробуем декодировать как UTF-8
        try:
            content.decode('utf-8')
            # Если успешно декодируется как UTF-8, то это не Windows-1251
            # (если только файл не содержит только ASCII символы, которые одинаковы в обеих кодировках)
            
            # Проверяем наличие русских символов в UTF-8
            try:
                text = content.decode('utf-8')
                # Ищем символы вне ASCII (>= 128)
                non_ascii = [c for c in text if ord(c) > 127]
                if non_ascii:
                    # Если есть не-ASCII символы и файл валидный UTF-8, то это UTF-8
                    return False
            except:
                pass
                
        except UnicodeDecodeError:
            # Невалидный UTF-8 - возможно Windows-1251
            pass
        
        # Пробуем декодировать как Windows-1251 и ищем русские символы
        try:
            text = content.decode('windows-1251')
            
            # Ищем русские символы в Windows-1251
            # Русские символы в Windows-1251 имеют байты от 0xC0 до 0xFF
            has_russian = any(b >= 0xC0 for b in content)
            
            # Также ищем специфичные для Windows-1251 последовательности
            # (буквы Ё, ё имеют байты 0xA8 и 0xB8)
            has_yo = any(b in (0xA8, 0xB8) for b in content)
            
            return has_russian or has_yo
            
        except:
            return False
            
    except Exception:
        return False

def convert_file(file_path):
    """
    Преобразует файл из Windows-1251 в UTF-8 с BOM
    """
    try:
        with open(file_path, 'rb') as f:
            content = f.read()
        
        # Декодируем как Windows-1251
        text = content.decode('windows-1251')
        
        # Записываем как UTF-8 с BOM
        with open(file_path, 'wb') as f:
            f.write(b'\xef\xbb\xbf')
            f.write(text.encode('utf-8'))
        
        return True
        
    except Exception:
        return False

def main():
    # Определяем директорию для обхода
    if len(sys.argv) > 1:
        root_dir = sys.argv[1]
    else:
        root_dir = "."
    
    root_path = Path(root_dir)
    
    if not root_path.exists():
        print(f"Ошибка: директория '{root_dir}' не найдена")
        return
    
    # Собираем все .h и .cpp файлы
    files = []
    for ext in ['*.h', '*.cpp']:
        files.extend(root_path.rglob(ext))
    
    if not files:
        print("Файлы .h и .cpp не найдены")
        return
    
    # Обрабатываем файлы
    converted_count = 0
    
    for file_path in sorted(files):
        if is_windows_1251(file_path):
            if convert_file(file_path):
                rel_path = file_path.relative_to(root_path) if root_dir != "." else file_path.name
                print(f"Преобразован: {rel_path}")
                converted_count += 1
    
    # Выводим итог
    if converted_count > 0:
        print(f"\nВсего преобразовано файлов: {converted_count}")
    else:
        print("Файлы в кодировке Windows-1251 не найдены")

if __name__ == "__main__":
    main()