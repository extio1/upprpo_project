#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <boost/archive/text_iarchive.hpp>

#include "document/document.h"

#include "compositor/simple_compositor/simple_compositor.h"

#include "executor/executor.h"
#include "executor/command/insert_character.h"
#include "executor/command/remove_character.h"
#include "executor/command/save_document.h"
#include "executor/command/load_document.h"
#include "executor/command/copy.h"
#include "executor/command/paste.h"

#include "executor/executor.h"
#include "executor/command/insert_character.h"
#include "executor/command/remove_character.h"
#include "executor/command/move_cursor_left.h"
#include "executor/command/move_cursor_right.h"

#include "compositor/simple_compositor/simple_compositor.h"

int main() {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    auto document = std::dynamic_pointer_cast<IDocument>(d);
    auto controller = std::make_unique<Executor>(2);

    controller->Do(std::make_shared<InsertCharacter>(document, 'H'));
    controller->Do(std::make_shared<InsertCharacter>(document, 'E'));
    controller->Do(std::make_shared<InsertCharacter>(document, 'L'));
    controller->Do(std::make_shared<InsertCharacter>(document, 'L'));
    controller->Do(std::make_shared<InsertCharacter>(document, '0'));

    controller->Undo();
    controller->Redo();

    std::cout << "Source: " << document << "\n";
    controller->Do(std::make_shared<SaveDocument>(document, "doc_save.file"));
    std::cout << "Saved: " << document << "\n";

    controller->Do(std::make_shared<RemoveCharacter>(document));
    controller->Do(std::make_shared<RemoveCharacter>(document));
    controller->Do(std::make_shared<RemoveCharacter>(document));
    controller->Do(std::make_shared<RemoveCharacter>(document));

    controller->Do(std::make_shared<LoadDocument>(&document, "doc_save.file"));
    std::cout << "Loaded: " << document << "\n";
    document->DrawDocument();

    controller->Do(std::make_shared<InsertCharacter>(document, ' '));

    controller->Do(std::make_shared<Copy>(document, Point(3, 5), Point(8, 5)));
    controller->Do(std::make_shared<Paste>(document, Point(9, 5)));

    controller->Do(std::make_shared<InsertCharacter>(document, '!'));
    std::cout << "UNDO !\n";
    controller->Undo();
    std::cout << "UNDO PASTE\n";
    controller->Undo();
    std::cout << "REDO PASTE\n";
    controller->Redo();

    controller->Do(std::make_shared<MoveCursorLeft>(document));
    std::cout << "MOVE CURSOR LEFT\n";
    document->DrawDocument();
    controller->Do(std::make_shared<InsertCharacter>(document, 'X'));

    controller->Do(std::make_shared<MoveCursorRight>(document));
    std::cout << "MOVE CURSOR RIGHT\n";
    document->DrawDocument();

    controller->Do(std::make_shared<MoveCursorRight>(document));
    std::cout << "MOVE CURSOR RIGHT\n";
    document->DrawDocument();

    return 0;
}