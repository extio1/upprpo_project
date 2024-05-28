#include "document/document.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(Document)

#include <algorithm>
#include <cassert>

#include "compositor/compositor.h"
#include "document/glyphs/character.h"
#include "document/glyphs/column.h"
#include "document/glyphs/glyph.h"
#include "document/glyphs/page.h"
#include "document/glyphs/row.h"

Document::Document(std::shared_ptr<Compositor> compositor) {
    currentPage = std::make_shared<Page>(0, 0, pageWidth, pageHeight);
    AddPage(currentPage);
    // set cursor on the first row on page
    selectedGlyph = this->GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();
    this->updateCursor();

    this->compositor = compositor;
    compositor->SetDocument(this);
    compositor->Compose();
}

void Document::SetCompositor(std::shared_ptr<Compositor> compositor) {
    this->compositor = compositor;
    compositor->SetDocument(this);
    compositor->Compose();
}

std::shared_ptr<Compositor> Document::GetCompositor() {
    return this->compositor;
}

Glyph::GlyphPtr Document::GetSelectedGlyph() { return selectedGlyph; }

void Document::updateCursor() {
    // check if selectedGlyph is a row or a character

    Row::RowPtr selectedRow = std::dynamic_pointer_cast<Row>(selectedGlyph);
    if (selectedRow != nullptr) {
        // set cursor in the beginning of this row
        Point cursorPoint =
            Point(selectedRow->GetPosition().x, selectedRow->GetPosition().y);
        // window->SetCursor(cursorPoint);
    } else {
        Character::CharPtr selectedChar =
            std::dynamic_pointer_cast<Character>(selectedGlyph);
        assert(selectedChar != nullptr && "Selected glyph has invalid type");
        Point cursorPoint =
            Point(selectedChar->GetPosition().x + selectedChar->GetWidth(),
                  selectedChar->GetPosition().y);
        // window->SetCursor(cursorPoint);
    }
}

void Document::Insert(Glyph::GlyphPtr& glyph) {
    currentPage->Insert(glyph);
    compositor->Compose();

    selectedGlyph = glyph;
    this->updateCursor();
    // glyph->Draw();
}

void Document::Remove(Glyph::GlyphPtr& glyph) {
    assert(glyph != nullptr && "Cannot remove glyph by nullptr");
    auto it = std::find(pages.begin(), pages.end(), glyph);
    if (it != pages.end()) {
        if (it != pages.begin()) pages.erase(it);
        return;
    }

    // what if this glyph is not from current page ???? glyph won't be found and
    // assertion will failed
    assert(glyph != nullptr && "Cannot remove glyph by nullptr");
    currentPage->Remove(glyph);
    compositor->Compose();
}

void Document::SetCurrentPage(Page::PagePtr page) {
    currentPage = std::move(page);
}

Page::PagePtr Document::GetCurrentPage() { return currentPage; }

size_t Document::GetPagesCount() const { return pages.size(); }

size_t Document::GetPageWidth() const { return pageWidth; }

size_t Document::GetPageHeight() const { return pageHeight; }

void Document::AddPage(const Page::PagePtr& page) {
    pages.push_back(page);
    // if (compositor) {
    // compositor->Compose();  // page can be non-formated
    // }
}

Page::PagePtr Document::GetFirstPage() { return *(pages.begin()); }

Page::PagePtr Document::GetNextPage(const Page::PagePtr& pagePtr) {
    auto currentPage =
        std::find_if(pages.begin(), pages.end(),
                     [&](const auto& elem) { return elem == pagePtr; });

    assert(currentPage != pages.end());

    auto nextPage = std::next(currentPage);
    if (nextPage == pages.end()) {
        return nullptr;
    }
    return std::static_pointer_cast<Page>(*nextPage);
}

void Document::SelectGlyphs(const Point& start, const Point& end) {
    Glyph::GlyphPtr area = std::make_shared<Column>(
        Column(start.x, start.y,
               (end.x > start.x ? end.x - start.x - 1 : end.x - start.x),
               (end.y > start.y ? end.y - start.y - 1 : end.y - start.y)));
    GlyphContainer::GlyphList list;
    Glyph::GlyphList glyphs = currentPage->Select(area);
    std::cout << "Selected glyphs:" << std::endl;
    for (auto glyph : glyphs) {
        std::cout << *glyph << std::endl;
    }

    selectedGlyphs.clear();
    for (auto& glyph : glyphs) {
        selectedGlyphs.push_back(glyph);
    }
}

Glyph::GlyphList Document::PasteGlyphs(const Point& to_point) {
    int currentX = to_point.x;
    int currentY = to_point.y;
    Glyph::GlyphPtr glyph;
    Glyph::GlyphList copiesList;
    for (auto& glyph : selectedGlyphs) {
        Glyph::GlyphPtr copy = glyph->Clone();
        copy->SetPosition(Point(currentX, currentY));  // set new position
        this->Insert(copy);
        copiesList.push_back(copy);
        currentX = copy->GetPosition().x +
                   copy->GetWidth();  // insert next glyph after this
        currentY =
            copy->GetPosition().y + 1;  // insert next glyph to the same row
    }
    // selected glyphs is not removed from selectedGlyphs, they can be pasted or
    // cut one more time
    return copiesList;
}

void Document::CutGlyphs(const Point& start, const Point& end) {
    SelectGlyphs(start, end);
    for (auto& glyph : selectedGlyphs) {
        this->Remove(glyph);
    }
}