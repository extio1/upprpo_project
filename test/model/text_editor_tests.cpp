#include <gtest/gtest.h>

#include <cstdlib>

#include "compositor/compositor.h"
#include "compositor/simple_compositor/simple_compositor.h"
#include "document/document.h"
#include "document/glyphs/character.h"
#include "document/glyphs/glyph.h"
#include "document/glyphs/row.h"

//----------------------------------------Glyph---------------------------------------------------
TEST(Glyph_Constructor, GlyphConstructor_WhenCalled_CreatesGlyphWithPosition) {
    Character c = Character(1, 2, 3, 4, 'A');
    ASSERT_EQ(c.GetPosition().x, 1);
    ASSERT_EQ(c.GetPosition().y, 2);
    ASSERT_EQ(c.GetWidth(), 3);
    ASSERT_EQ(c.GetHeight(), 4);
}

TEST(Glyph_Intersects_Point1,
     GlyphIntersectsInternPoints_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 2, 2, 'A');
    // corners
    ASSERT_TRUE(c.Intersects(Point(1, 2)));
    ASSERT_TRUE(c.Intersects(Point(3, 2)));
    ASSERT_TRUE(c.Intersects(Point(1, 4)));
    ASSERT_TRUE(c.Intersects(Point(3, 4)));
    // inside
    ASSERT_TRUE(c.Intersects(Point(2, 2)));
    ASSERT_TRUE(c.Intersects(Point(1, 3)));
    ASSERT_TRUE(c.Intersects(Point(2, 3)));
    ASSERT_TRUE(c.Intersects(Point(3, 3)));
    ASSERT_TRUE(c.Intersects(Point(2, 4)));
}

TEST(Glyph_Intersects_Point2,
     GlyphIntersectsExternPoints_WhenCalled_ReturnFalse) {
    Character c = Character(1, 2, 2, 2, 'A');
    // corners
    ASSERT_FALSE(c.Intersects(Point(0, 0)));
    ASSERT_FALSE(c.Intersects(Point(0, 1)));
    ASSERT_FALSE(c.Intersects(Point(1, 1)));
    ASSERT_FALSE(c.Intersects(Point(2, 1)));
    // inside
    ASSERT_FALSE(c.Intersects(Point(3, 1)));
    ASSERT_FALSE(c.Intersects(Point(4, 1)));
    ASSERT_FALSE(c.Intersects(Point(4, 2)));
    ASSERT_FALSE(c.Intersects(Point(4, 2)));
    ASSERT_FALSE(c.Intersects(Point(4, 4)));
    ASSERT_FALSE(c.Intersects(Point(4, 5)));
    ASSERT_FALSE(c.Intersects(Point(3, 5)));
    ASSERT_FALSE(c.Intersects(Point(2, 5)));
    ASSERT_FALSE(c.Intersects(Point(1, 5)));
    ASSERT_FALSE(c.Intersects(Point(0, 5)));
    ASSERT_FALSE(c.Intersects(Point(0, 4)));
    ASSERT_FALSE(c.Intersects(Point(0, 3)));
    ASSERT_FALSE(c.Intersects(Point(0, 2)));
}

TEST(Glyph_Intersects_Glyph1,
     GlyphIntersectsTheSameGlyph_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 2, 2, 'A');
    ASSERT_TRUE(c.Intersects(std::make_shared<Character>(c)));
}

TEST(Glyph_Intersects_Glyph2,
     GlyphIntersectsSmallerGlyph_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 3, 3, 'A');
    Character c2 = Character(2, 3, 1, 1, 'B');
    ASSERT_TRUE(c.Intersects(std::make_shared<Character>(c2)));
}

TEST(Glyph_Intersects_Glyph3,
     GlyphIntersectsBiggerGlyph_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 3, 3, 'A');
    Character c2 = Character(0, 0, 10, 10, 'B');
    ASSERT_TRUE(c.Intersects(std::make_shared<Character>(c2)));
}

TEST(Glyph_Intersects_Glyph4,
     GlyphIntersectsGlyphAround_WhenCalled_ReturnFalse) {
    Character c = Character(1, 2, 3, 3, 'A');
    Character c2 = Character(0, 0, 1, 1, 'B');
    ASSERT_FALSE(c.Intersects(std::make_shared<Character>(c2)));
}

TEST(Glyph_Intersects_Glyph5,
     GlyphIntersectsGlyphByCorner_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 3, 3, 'A');
    Character c2 = Character(3, 4, 1, 1, 'B');
    ASSERT_TRUE(c.Intersects(std::make_shared<Character>(c2)));
}

TEST(Glyph_Intersects_Glyph6,
     ZeroSizeGlyphIntersetsHimself_WhenCalled_ReturnTrue) {
    Character c = Character(1, 2, 0, 0, 'A');
    ASSERT_TRUE(c.Intersects(std::make_shared<Character>(c)));
}

TEST(Glyph_MoveGlyph1,
     GlyphMoveGlyph_WhenCalled_ChangesPositionOfGlyphDueArguments) {
    Character c = Character(1, 2, 3, 4, 'A');
    c.MoveGlyph(1, 2);
    ASSERT_EQ(c.GetPosition().x, 2);
    ASSERT_EQ(c.GetPosition().y, 4);
}

TEST(Glyph_MoveGlyph2,
     GlyphMoveGlyphZero_WhenCalled_NotChangesPositionOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');
    c.MoveGlyph(0, 0);
    ASSERT_EQ(c.GetPosition().x, 1);
    ASSERT_EQ(c.GetPosition().y, 2);
}

TEST(Glyph_MoveGlyph3,
     GlyphMoveGlyphNegative_WhenCalled_ChangesPositionOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');
    c.MoveGlyph(-1, -1);
    ASSERT_EQ(c.GetPosition().x, 0);
    ASSERT_EQ(c.GetPosition().y, 1);
}

TEST(Glyph_MoveGlyph4, GlyphMoveGlyphNegative_WhenCalled_AssertFailed) {
    Character c = Character(1, 2, 3, 4, 'A');
    ASSERT_DEATH(c.MoveGlyph(-2, -3),
                 "Cannot move glyph due to these coordinates");
}

TEST(Glyph_SetPosition1, GlyphSetPosition_WhenCalled_ChangesPositionOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetPosition({6, 7});
    ASSERT_EQ(c.GetPosition().x, 6);
    ASSERT_EQ(c.GetPosition().y, 7);
}

TEST(Glyph_SetPosition2, GlyphSetPosition_WhenCalled_ChangesPositionOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetPosition(6, 7);
    ASSERT_EQ(c.GetPosition().x, 6);
    ASSERT_EQ(c.GetPosition().y, 7);
}

TEST(Glyph_SetWidth1, GlyphSetWidth_WhenCalled_ChangesWidthOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetWidth(5);
    ASSERT_EQ(c.GetPosition().x, 1);
    ASSERT_EQ(c.GetPosition().y, 2);
    ASSERT_EQ(c.GetWidth(), 5);
    ASSERT_EQ(c.GetHeight(), 4);
}

TEST(Glyph_SetWidth2, GlyphSetWidth_WhenCalled_AssertFailed) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_DEATH(c.SetWidth(-1), "Invalid width of glyph");
}

TEST(Glyph_SetHeight, GlyphSetHeight_WhenCalled_ChangesHeightOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetHeight(5);
    ASSERT_EQ(c.GetPosition().x, 1);
    ASSERT_EQ(c.GetPosition().y, 2);
    ASSERT_EQ(c.GetWidth(), 3);
    ASSERT_EQ(c.GetHeight(), 5);
}

TEST(Glyph_SetHeight2, GlyphSetHeight_WhenCalled_AssertFailed) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_DEATH(c.SetHeight(-1), "Invalid height of glyph");
}

TEST(Glyph_SetParams1, GlyphSetParams_WhenCalled_ChangesParamsOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetGlyphParams(5, 6, 7, 8);
    ASSERT_EQ(c.GetPosition().x, 5);
    ASSERT_EQ(c.GetPosition().y, 6);
    ASSERT_EQ(c.GetWidth(), 7);
    ASSERT_EQ(c.GetHeight(), 8);
}

TEST(Glyph_SetParams2, GlyphSetParams_WhenCalled_AssertFailed) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_DEATH(c.SetGlyphParams(-5, 6, 7, 8), "Invalid params of glyph");
}

TEST(Glyph_GetBorders,
     GlyphGetBorders_WhenCalled_ReturnsBottomAndRightBordersOfGlyph) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_EQ(c.GetBottomBorder(), 6);
    ASSERT_EQ(c.GetRightBorder(), 4);
}

//---------------------------------------Character------------------------------------------------------

TEST(Character_Constructor, CharacterConstructor_WhenCalled_CreatesCharacter) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_EQ(c.GetPosition().x, 1);
    ASSERT_EQ(c.GetPosition().y, 2);
    ASSERT_EQ(c.GetWidth(), 3);
    ASSERT_EQ(c.GetHeight(), 4);
    ASSERT_EQ(c.GetChar(), 'A');
}

TEST(Character_GetChar, CharacterGetChar_WhenCalled_ReturnsSymbol) {
    Character c = Character(1, 2, 3, 4, 'A');

    ASSERT_EQ(c.GetChar(), 'A');
}

TEST(Character_SetChar, CharacterSetChar_WhenCalled_ChangesSymbol) {
    Character c = Character(1, 2, 3, 4, 'A');

    c.SetChar('F');
    ASSERT_EQ(c.GetChar(), 'F');
}

//-------------------------------------GlyphContainer-------------------------------------------------

TEST(GlyphContainer_Constructor,
     GlyphContainerConstructor_WhenCalled_CreatesGlyphContainer) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);

    ASSERT_EQ(rowPtr->GetPosition().x, 1);
    ASSERT_EQ(rowPtr->GetPosition().y, 2);
    ASSERT_EQ(rowPtr->GetWidth(), 3);
    ASSERT_EQ(rowPtr->GetHeight(), 4);
}

TEST(GlyphContainer_GetGlyphIndex1,
     GlyphContainerGetGlyphIndex_WhenCalled_ReturnsIndexOfGlyph) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);

    ASSERT_EQ(rowPtr->GetGlyphIndex(cPtr), 0);
}

TEST(GlyphContainer_GetGlyphIndex2,
     GlyphContainerGetGlyphIndex_WhenCalled_ReturnsIndexOfGlyph) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);
    ASSERT_EQ(rowPtr->GetGlyphIndex(cPtr), 0);

    Character c2 = Character(1, 2, 3, 4, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    rowPtr->Add(c2Ptr);
    ASSERT_EQ(rowPtr->GetGlyphIndex(cPtr), 0);
    ASSERT_EQ(rowPtr->GetGlyphIndex(c2Ptr), 1);
}

TEST(GlyphContainer_GetGlyphIndex3,
     GlyphContainerGetGlyphIndex_WhenCalled_AssertFailed) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);

    ASSERT_DEATH(rowPtr->GetGlyphIndex(cPtr),
                 "GlyphContainer doesn't contain this glyph");
}

TEST(GlyphContainer_GetGlyphByIndex1,
     GlyphContainerGetGlyphByIndex_WhenCalled_ReturnsGlyphPtr) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);

    ASSERT_EQ(rowPtr->GetGlyphByIndex(0), cPtr);
}

TEST(GlyphContainer_GetGlyphByIndex2,
     GlyphContainerGetGlyphByIndex_WhenCalled_ReturnsNullPtr) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);

    ASSERT_EQ(rowPtr->GetGlyphByIndex(1), nullptr);
}

TEST(GlyphContainer_GetGlyphByIndex3,
     GlyphContainerGetGlyphByIndex_WhenCalled_ReturnsNullPtr) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);

    ASSERT_DEATH(rowPtr->GetGlyphByIndex(-1), "Invalid index of glyph");
}

// TEST(GlyphContainer_Find1, GlyphContainerFind_WhenCalled_ReturnsGlyphPtr) {
//     Row r = Row(1, 2, 3, 4);
//     std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
//     Character c = Character(1, 2, 3, 4, 'A');
//     Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
//     rowPtr->Add(cPtr);

//     Character c2 = Character(5, 6, 7, 8, 'B');
//     Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
//     rowPtr->Add(c2Ptr);

//     ASSERT_EQ(rowPtr->Find(Point(1, 2)), cPtr);
// }

// TEST(GlyphContainer_Find2, GlyphContainerFind_WhenCalled_ReturnsGlyphPtr) {
//     Row r = Row(1, 2, 3, 4);
//     std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
//     Character c = Character(1, 2, 3, 4, 'A');
//     Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
//     rowPtr->Add(cPtr);

//     Character c2 = Character(5, 6, 7, 8, 'B');
//     Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
//     rowPtr->Add(c2Ptr);

//     ASSERT_EQ(rowPtr->Find(Point(0, 0)), nullptr);
// }

// TEST(GlyphContainer_Find3,
// GlyphContainerFind_WhenCalled_ReturnsFirstGlyphPtr) {
//     Row r = Row(1, 2, 3, 4);
//     std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
//     Character c = Character(1, 2, 3, 4, 'A');
//     Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
//     rowPtr->Add(cPtr);

//     Character c2 = Character(1, 2, 3, 4, 'B');
//     Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
//     rowPtr->Add(c2Ptr);

//     ASSERT_EQ(rowPtr->Find(Point(1, 2)), cPtr);
// }

TEST(GlyphContainer_Add1, GlyphContainerAdd_WhenCalled_AddsGlyph) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c = Character(1, 2, 3, 4, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    rowPtr->Add(cPtr);

    ASSERT_EQ(rowPtr->GetGlyphIndex(cPtr), 0);
}

TEST(GlyphContainer_MoveGlyph1,
     GlyphContainerMoveGlyph_WhenCalled_MoveAllGlyphsInComponents) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c1 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(35, 10, 5, 5, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    rowPtr->Insert(c1Ptr);
    rowPtr->Insert(c2Ptr);

    rowPtr->MoveGlyph(1, 2);

    Glyph::GlyphPtr c1PtrNew = rowPtr->GetGlyphByIndex(0);
    Glyph::GlyphPtr c2PtrNew = rowPtr->GetGlyphByIndex(1);

    ASSERT_EQ(c1PtrNew->GetPosition().x, 31);
    ASSERT_EQ(c1PtrNew->GetPosition().y, 12);
    ASSERT_EQ(c1PtrNew->GetWidth(), 5);
    ASSERT_EQ(c1PtrNew->GetHeight(), 5);

    ASSERT_EQ(c2PtrNew->GetPosition().x, 36);
    ASSERT_EQ(c2PtrNew->GetPosition().y, 12);
    ASSERT_EQ(c2PtrNew->GetWidth(), 5);
    ASSERT_EQ(c2PtrNew->GetHeight(), 5);
}

TEST(GlyphContainer_MoveGlyph2,
     GlyphContainerMoveGlyph_WhenCalled_AssertFailed) {
    Row r = Row(1, 2, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);

    ASSERT_DEATH(rowPtr->MoveGlyph(-2, 1),
                 "Cannot move glyph due to these coordinates");
}

TEST(GlyphContainer_MoveGlyph3,
     GlyphContainerMoveGlyph_WhenCalled_MoveAllGlyphsInComponents) {
    Row r = Row(5, 5, 3, 4);
    std::shared_ptr<GlyphContainer> rowPtr = std::make_shared<Row>(r);
    Character c1 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(35, 10, 5, 5, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    rowPtr->Insert(c1Ptr);
    rowPtr->Insert(c2Ptr);

    ASSERT_DEATH(rowPtr->MoveGlyph(-31, 1),
                 "Cannot move glyph due to these coordinates");
}

// -----------------------------Row------------------------------------------------------

TEST(Row_Constructor, RowConstructor_WhenCalled_CreatesEmptyRow) {
    Row r = Row(1, 2, 3, 4);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 3);
    ASSERT_EQ(r.GetHeight(), 4);
    ASSERT_EQ(r.GetGlyphByIndex(0), nullptr);
    ASSERT_EQ(r.GetUsedSpace(), 0);
    ASSERT_EQ(r.GetFreeSpace(), 3);
}

TEST(Row_Insert1, RowInsertGlyph_WhenCalled_InsertsGlyphInEmptyRowByPosition) {
    Row r = Row(1, 2, 5, 1);

    Character c = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);

    r.Insert(cPtr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0), cPtr);
    ASSERT_EQ(r.GetGlyphIndex(cPtr), 0);
    ASSERT_EQ(r.GetUsedSpace(), 1);
    ASSERT_EQ(r.GetFreeSpace(), 4);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(Row_Insert2, RowInsertGlyphWithTheSameSize_WhenCalled_BecomeFull) {
    Row r = Row(1, 2, 5, 1);

    Character c = Character(1, 2, 5, 1, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);

    r.Insert(cPtr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0), cPtr);
    ASSERT_EQ(r.GetGlyphIndex(cPtr), 0);
    ASSERT_EQ(r.GetUsedSpace(), 5);
    ASSERT_EQ(r.GetFreeSpace(), 0);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_TRUE(r.IsFull());
}

TEST(Row_Insert2, RowInsertBigGlyph_WhenCalled_CannotInsertBigGlyph) {
    Row r = Row(1, 2, 5, 1);

    Character c = Character(1, 2, 6, 1, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    r.Insert(cPtr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0), cPtr);
    ASSERT_EQ(r.GetGlyphIndex(cPtr), 0);
    ASSERT_EQ(r.GetUsedSpace(), 6);
    ASSERT_EQ(r.GetFreeSpace(), -1);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_TRUE(r.IsFull());
}

TEST(Row_Insert3, RowInsertTheSameGlyph_WhenCalled_InsertGlyphBeforeExisting) {
    Row r = Row(1, 2, 5, 1);

    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    r.Insert(c1Ptr);

    Character c2 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    r.Insert(c2Ptr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0), c2Ptr);
    ASSERT_EQ(r.GetGlyphIndex(c2Ptr), 0);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetWidth(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetHeight(), 1);

    ASSERT_EQ(r.GetGlyphByIndex(1), c1Ptr);
    ASSERT_EQ(r.GetGlyphIndex(c1Ptr), 1);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetWidth(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetHeight(), 1);

    ASSERT_EQ(r.GetUsedSpace(), 2);
    ASSERT_EQ(r.GetFreeSpace(), 3);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

// add test for wrong position glyph insert
TEST(Row_Insert4, RowInsertDueToWrongPosition_WhenCalled_AssertFailed) {
    Row r = Row(1, 2, 5, 1);

    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    r.Insert(c1Ptr);

    Character c2 = Character(4, 2, 1, 1, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    ASSERT_DEATH(r.Insert(c2Ptr),
                 "No suitable character for inserting next to");

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0), c1Ptr);
    ASSERT_EQ(r.GetGlyphIndex(c1Ptr), 0);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetWidth(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetHeight(), 1);

    ASSERT_EQ(r.GetUsedSpace(), 1);
    ASSERT_EQ(r.GetFreeSpace(), 4);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(Row_Remove1, RowRemoveTheOnlyGlyph_WhenCalled_RowBecomesEmpty) {
    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);

    Row r = Row(1, 2, 5, 1);
    r.Insert(c1Ptr);

    r.Remove(c1Ptr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_DEATH(r.GetGlyphIndex(c1Ptr),
                 "GlyphContainer doesn't contain this glyph");
    ASSERT_EQ(r.GetGlyphByIndex(0), nullptr);

    ASSERT_EQ(r.GetUsedSpace(), 0);
    ASSERT_EQ(r.GetFreeSpace(), 5);
    ASSERT_TRUE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(Row_Remove2, RowRemoveGlyphFromTheEnd_WhenCalled_RemovesGlyphFromRow) {
    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(1, 2, 2, 1, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);

    Row r = Row(1, 2, 5, 1);
    r.Insert(c1Ptr);
    r.Insert(c2Ptr);

    std::cout << "1" << std::endl;
    r.Remove(c1Ptr);
    std::cout << "2" << std::endl;

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphIndex(c2Ptr), 0);
    ASSERT_DEATH(r.GetGlyphIndex(c1Ptr),
                 "GlyphContainer doesn't contain this glyph");
    ASSERT_EQ(r.GetGlyphByIndex(0), c2Ptr);
    ASSERT_EQ(r.GetGlyphByIndex(1), nullptr);

    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetWidth(), 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetHeight(), 1);

    ASSERT_EQ(r.GetUsedSpace(), 2);
    ASSERT_EQ(r.GetFreeSpace(), 3);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(
    Row_Remove3,
    RowRemoveGlyphFromTheBegin_WhenCalled_RemovesGlyphFromRowAndMoveRestGlyphs) {
    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(1, 2, 2, 1, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);

    Row r = Row(1, 2, 5, 1);
    r.Insert(c1Ptr);
    r.Insert(c2Ptr);

    r.Remove(c2Ptr);

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphIndex(c1Ptr), 0);
    ASSERT_DEATH(r.GetGlyphIndex(c2Ptr),
                 "GlyphContainer doesn't contain this glyph");
    ASSERT_EQ(r.GetGlyphByIndex(0), c1Ptr);
    ASSERT_EQ(r.GetGlyphByIndex(1), nullptr);

    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetWidth(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetHeight(), 1);

    ASSERT_EQ(r.GetUsedSpace(), 1);
    ASSERT_EQ(r.GetFreeSpace(), 4);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(Row_Remove4, RowRemoveNullPtr_WhenCalled_RemovesNothing) {
    Character c1 = Character(1, 2, 1, 1, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(1, 2, 2, 1, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);

    Row r = Row(1, 2, 5, 1);
    r.Insert(c1Ptr);
    r.Insert(c2Ptr);

    ASSERT_DEATH(r.Remove(nullptr), "Cannot remove glyph by nullptr");

    ASSERT_EQ(r.GetPosition().x, 1);
    ASSERT_EQ(r.GetPosition().y, 2);
    ASSERT_EQ(r.GetWidth(), 5);
    ASSERT_EQ(r.GetHeight(), 1);
    ASSERT_EQ(r.GetGlyphIndex(c1Ptr), 1);
    ASSERT_EQ(r.GetGlyphIndex(c2Ptr), 0);
    ASSERT_EQ(r.GetGlyphByIndex(0), c2Ptr);
    ASSERT_EQ(r.GetGlyphByIndex(1), c1Ptr);

    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetWidth(), 2);
    ASSERT_EQ(r.GetGlyphByIndex(0)->GetHeight(), 1);

    ASSERT_EQ(r.GetGlyphByIndex(1)->GetPosition().x, 1);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetPosition().y, 2);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetWidth(), 1);
    ASSERT_EQ(r.GetGlyphByIndex(1)->GetHeight(), 1);

    ASSERT_EQ(r.GetUsedSpace(), 3);
    ASSERT_EQ(r.GetFreeSpace(), 2);
    ASSERT_FALSE(r.IsEmpty());
    ASSERT_FALSE(r.IsFull());
}

TEST(SimpleCompositor_Compose1,
     SimpleCompositorCompose_WhenCalled_ComposeAllGlyphInDocumentByDefault) {
    Document document(std::make_shared<SimpleCompositor>());

    EXPECT_EQ(document.GetPagesCount(), 1);

    // one page with one column with one row in document was added after in
    // constructor
    EXPECT_EQ(document.GetFirstPage()->GetColumnsCount(), 1);
    Glyph::GlyphPtr firstColumn = document.GetFirstPage()->GetFirstGlyph();
    EXPECT_EQ(document.GetFirstPage()->GetNextGlyph(firstColumn), nullptr);
    EXPECT_EQ(document.GetFirstPage()->GetWidth(), 500);
    EXPECT_EQ(document.GetFirstPage()->GetHeight(), 1000);

    // empty first row in first column in first page
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              nullptr);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              3);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              5);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        491);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        1);

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 3);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 5);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 491);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 985);
}

TEST(
    SimpleCompositor_Compose2,
    SimpleCompositorCompose_WhenCalled_ComposeAllGlyphInDocumentBySpecifiedParams) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    EXPECT_EQ(document.GetPagesCount(), 1);

    // one page with one column with one row in document was added after in
    // constructor
    EXPECT_EQ(document.GetFirstPage()->GetColumnsCount(), 1);
    Glyph::GlyphPtr firstColumn = document.GetFirstPage()->GetFirstGlyph();
    EXPECT_EQ(document.GetFirstPage()->GetNextGlyph(firstColumn), nullptr);
    EXPECT_EQ(document.GetFirstPage()->GetWidth(), 500);
    EXPECT_EQ(document.GetFirstPage()->GetHeight(), 1000);

    // empty first row in first column in first page
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              nullptr);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        1);

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(Document_Insert,
     DocumentInsert_WhenCalled_InsertGlyphByItsPositionAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c = Character(30, 10, 1, 1, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    document.Insert(cPtr);

    EXPECT_EQ(document.GetPagesCount(), 1);

    EXPECT_EQ(document.GetFirstPage()->GetColumnsCount(), 1);
    Glyph::GlyphPtr firstColumn = document.GetFirstPage()->GetFirstGlyph();
    EXPECT_EQ(document.GetFirstPage()->GetNextGlyph(firstColumn), nullptr);
    EXPECT_EQ(document.GetFirstPage()->GetWidth(), 500);
    EXPECT_EQ(document.GetFirstPage()->GetHeight(), 1000);

    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              cPtr);
    // check character params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetWidth(),
              1);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetHeight(),
              1);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        1);

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(Document_Insert2,
     DocumentInsert_WhenCalled_InsertGlyphByItsPositionAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c = Character(40, 10, 5, 5, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    document.Insert(cPtr);

    EXPECT_EQ(document.GetPagesCount(), 1);

    EXPECT_EQ(document.GetFirstPage()->GetColumnsCount(), 1);
    Glyph::GlyphPtr firstColumn = document.GetFirstPage()->GetFirstGlyph();
    EXPECT_EQ(document.GetFirstPage()->GetNextGlyph(firstColumn), nullptr);
    EXPECT_EQ(document.GetFirstPage()->GetWidth(), 500);
    EXPECT_EQ(document.GetFirstPage()->GetHeight(), 1000);

    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              cPtr);
    // check character params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetWidth(),
              5);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetHeight(),
              5);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        5);  // height of row was increased until height of character

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(
    Document_Insert3,
    DocumentInsert_WhenCalled_InsertGlyphDueToCenterAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    document.Insert(cPtr);

    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              cPtr);
    // check character params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              242);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetWidth(),
              5);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetHeight(),
              5);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        5);  // height of row was increased until height of character

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(Document_Insert4,
     DocumentInsert_WhenCalled_InsertGlyphDueToRightAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::RIGHT, 100));

    Character c = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    document.Insert(cPtr);

    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              cPtr);
    // check character params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              455);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetWidth(),
              5);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetHeight(),
              5);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        5);  // height of row was increased until height of character

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(
    Document_Insert5,
    DocumentInsert_WhenCalled_InsertGlyphDueToJustifiedAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::JUSTIFIED, 100));

    Character c = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    document.Insert(cPtr);

    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph(),
              cPtr);
    // check character params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetWidth(),
              5);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetHeight(),
              5);

    // check row params
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .x,
              30);
    EXPECT_EQ(document.GetFirstPage()
                  ->GetFirstGlyph()
                  ->GetFirstGlyph()
                  ->GetPosition()
                  .y,
              10);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetWidth(),
        430);
    EXPECT_EQ(
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetHeight(),
        5);  // height of row was increased until height of character

    // check column params
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().x, 30);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetPosition().y, 10);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetWidth(), 430);
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetHeight(), 970);
}

TEST(
    Document_Insert6,
    DocumentInsert_WhenCalled_InsertSeveralCharactersDueToLeftAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c3Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c1Ptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 35);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 40);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
}

TEST(
    Document_Insert7,
    DocumentInsert_WhenCalled_InsertSeveralCharactersDueToCenterAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(242, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(250, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c1Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c3Ptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 237);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 242);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 247);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
}

TEST(
    Document_Insert8,
    DocumentInsert_WhenCalled_InsertSeveralCharactersDueToRightAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::RIGHT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c1Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c2Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c3Ptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 445);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 450);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 455);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
}

TEST(
    Document_Insert9,
    DocumentInsert_WhenCalled_InsertWideCharactersDueToLeftAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 150, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 150, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 150, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr firstRow =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();

    Glyph::GlyphPtr firstChar = firstRow->GetFirstGlyph();
    EXPECT_EQ(firstChar, c3Ptr);
    Glyph::GlyphPtr secondChar = firstRow->GetNextGlyph(firstChar);
    EXPECT_EQ(secondChar, c2Ptr);
    Glyph::GlyphPtr movedChar = firstRow->GetNextGlyph(secondChar);
    EXPECT_EQ(movedChar, nullptr);

    Glyph::GlyphPtr secondRow =
        document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(firstRow);
    Glyph::GlyphPtr thirdChar = secondRow->GetFirstGlyph();
    EXPECT_EQ(thirdChar, c1Ptr);

    // check first character params
    EXPECT_EQ(firstChar->GetPosition().x, 30);
    EXPECT_EQ(firstChar->GetPosition().y, 10);
    EXPECT_EQ(firstChar->GetWidth(), 150);
    EXPECT_EQ(firstChar->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(secondChar->GetPosition().x, 180);
    EXPECT_EQ(secondChar->GetPosition().y, 10);
    EXPECT_EQ(secondChar->GetWidth(), 150);
    EXPECT_EQ(secondChar->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(thirdChar->GetPosition().x, 30);
    EXPECT_EQ(thirdChar->GetPosition().y,
              115);  // topIndent=10 + thirdChar.height=5 + lineSpacing=100
    EXPECT_EQ(thirdChar->GetWidth(), 150);
    EXPECT_EQ(thirdChar->GetHeight(), 5);
}

TEST(
    Document_Insert10,
    DocumentInsert_WhenCalled_InsertWideCharactersDueToCenterAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c1 = Character(100, 10, 150, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(320, 10, 150, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(395, 10, 150, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr firstRow =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();

    Glyph::GlyphPtr firstChar = firstRow->GetFirstGlyph();
    EXPECT_EQ(firstChar, c1Ptr);
    Glyph::GlyphPtr secondChar = firstRow->GetNextGlyph(firstChar);
    EXPECT_EQ(secondChar, c2Ptr);
    Glyph::GlyphPtr movedChar = firstRow->GetNextGlyph(secondChar);
    EXPECT_EQ(movedChar, nullptr);

    Glyph::GlyphPtr secondRow =
        document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(firstRow);
    Glyph::GlyphPtr thirdChar = secondRow->GetFirstGlyph();
    EXPECT_EQ(thirdChar, c3Ptr);

    // check first character params
    EXPECT_EQ(firstChar->GetPosition().x, 95);
    EXPECT_EQ(firstChar->GetPosition().y, 10);
    EXPECT_EQ(firstChar->GetWidth(), 150);
    EXPECT_EQ(firstChar->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(secondChar->GetPosition().x, 245);
    EXPECT_EQ(secondChar->GetPosition().y, 10);
    EXPECT_EQ(secondChar->GetWidth(), 150);
    EXPECT_EQ(secondChar->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(thirdChar->GetPosition().x, 170);
    EXPECT_EQ(thirdChar->GetPosition().y,
              115);  // topIndent=10 + thirdChar.height=5 + lineSpacing=100
    EXPECT_EQ(thirdChar->GetWidth(), 150);
    EXPECT_EQ(thirdChar->GetHeight(), 5);
}

TEST(Document_Insert11,
     DocumentInsert_WhenCalled_InsertCharactersIntoSeveralPages) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c1 = Character(100, 10, 150, 1000, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(320, 10, 150, 1000, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(395, 10, 150, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    Glyph::GlyphPtr firstRow =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();

    Glyph::GlyphPtr firstChar = firstRow->GetFirstGlyph();
    EXPECT_EQ(firstChar, c1Ptr);
    Glyph::GlyphPtr secondChar = firstRow->GetNextGlyph(firstChar);
    EXPECT_EQ(secondChar, c2Ptr);
    Glyph::GlyphPtr movedChar = firstRow->GetNextGlyph(secondChar);
    EXPECT_EQ(movedChar, nullptr);

    // only one row is on the first page
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(firstRow),
              nullptr);

    // second row on the next page
    Glyph::GlyphPtr secondRow = document.GetNextPage(document.GetFirstPage())
                                    ->GetFirstGlyph()
                                    ->GetFirstGlyph();
    Glyph::GlyphPtr thirdChar = secondRow->GetFirstGlyph();
    EXPECT_EQ(thirdChar, c3Ptr);

    // check first character params
    EXPECT_EQ(firstChar->GetPosition().x, 95);
    EXPECT_EQ(firstChar->GetPosition().y, 10);
    EXPECT_EQ(firstChar->GetWidth(), 150);
    EXPECT_EQ(firstChar->GetHeight(), 1000);

    // check second character params
    EXPECT_EQ(secondChar->GetPosition().x, 245);
    EXPECT_EQ(secondChar->GetPosition().y, 10);
    EXPECT_EQ(secondChar->GetWidth(), 150);
    EXPECT_EQ(secondChar->GetHeight(), 1000);

    // check third character params
    EXPECT_EQ(thirdChar->GetPosition().x, 170);
    EXPECT_EQ(thirdChar->GetPosition().y,
              10);  // top of second page
    EXPECT_EQ(thirdChar->GetWidth(), 150);
    EXPECT_EQ(thirdChar->GetHeight(), 5);
}

TEST(Document_Insert12,
     DocumentInsertDueToWrongPosition_WhenCalled_AssertFailed) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 150, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(181, 10, 150, 5,
                             'A');  // x = leftIndent=30 + charWidth=150 + 1
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    document.Insert(c1Ptr);
    ASSERT_DEATH(document.Insert(c2Ptr),
                 "No suitable character for inserting next to");
}

TEST(
    Document_Remove1,
    DocumentRemove_WhenCalled_RemoveCharactersWithLeftAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    EXPECT_EQ(c1Ptr->GetPosition().x, 40);
    EXPECT_EQ(c1Ptr->GetPosition().y, 10);
    EXPECT_EQ(c2Ptr->GetPosition().x, 30);
    EXPECT_EQ(c2Ptr->GetPosition().y, 10);
    EXPECT_EQ(c3Ptr->GetPosition().x, 35);
    EXPECT_EQ(c3Ptr->GetPosition().y, 10);

    document.Remove(c2Ptr);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c3Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c1Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, nullptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 35);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);
}

TEST(
    Document_Remove2,
    DocumentRemove_WhenCalled_RemoveCharactersWithCenterAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(242, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(250, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.Remove(c1Ptr);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c3Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, nullptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 240);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 245);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);
}

TEST(
    Document_Remove3,
    DocumentRemove_WhenCalled_RemoveCharactersWithRightAlignmentAndComposeItself) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::RIGHT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.Remove(c3Ptr);

    EXPECT_EQ(document.GetPagesCount(), 1);

    EXPECT_EQ(document.GetFirstPage()->GetColumnsCount(), 1);
    Glyph::GlyphPtr firstColumn = document.GetFirstPage()->GetFirstGlyph();
    EXPECT_EQ(document.GetFirstPage()->GetNextGlyph(firstColumn), nullptr);
    EXPECT_EQ(document.GetFirstPage()->GetWidth(), 500);
    EXPECT_EQ(document.GetFirstPage()->GetHeight(), 1000);

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c1Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c2Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, nullptr);  // removed

    // check first character params
    EXPECT_EQ(first->GetPosition().x, 450);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 455);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);
}

TEST(Document_Remove4, DocumentRemove_WhenCalled_RemoveNothingByNullptr) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::RIGHT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(460, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    EXPECT_EQ(c3Ptr->GetPosition().x, 455);
    EXPECT_EQ(c3Ptr->GetPosition().y, 10);

    Glyph::GlyphPtr c4Ptr = nullptr;

    ASSERT_DEATH(document.Remove(c4Ptr), "Cannot remove glyph by nullptr");
}

TEST(Document_Remove5, DocumentRemove_WhenCalled_RemoveFromEmptyDocument) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::RIGHT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);

    ASSERT_DEATH(document.Remove(c1Ptr), "No suitable character for removing");
}

TEST(Document_Remove6,
     DocumentRemove_WhenCalled_RemovesCharacterAndComposePages) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::CENTER, 100));

    Character c1 = Character(100, 10, 150, 1000, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(320, 10, 150, 1000, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(395, 10, 150, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.Remove(c1Ptr);

    Glyph::GlyphPtr firstRow =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();

    Glyph::GlyphPtr firstChar = firstRow->GetFirstGlyph();
    EXPECT_EQ(firstChar, c2Ptr);
    Glyph::GlyphPtr secondChar = firstRow->GetNextGlyph(firstChar);
    EXPECT_EQ(secondChar, c3Ptr);
    Glyph::GlyphPtr movedChar = firstRow->GetNextGlyph(secondChar);
    EXPECT_EQ(movedChar, nullptr);

    // only one row is on the first page
    EXPECT_EQ(document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(firstRow),
              nullptr);

    // empty second page was removed
    EXPECT_EQ(document.GetNextPage(document.GetFirstPage()), nullptr);

    // check first character params
    EXPECT_EQ(firstChar->GetPosition().x, 95);
    EXPECT_EQ(firstChar->GetPosition().y, 10);
    EXPECT_EQ(firstChar->GetWidth(), 150);
    EXPECT_EQ(firstChar->GetHeight(), 1000);

    // check second character params
    EXPECT_EQ(secondChar->GetPosition().x, 245);
    EXPECT_EQ(secondChar->GetPosition().y, 10);
    EXPECT_EQ(secondChar->GetWidth(), 150);
    EXPECT_EQ(secondChar->GetHeight(), 5);
}

TEST(Document_SelectGlyphs,
     DocumentSelectGlyphs_WhenCalled_SaveGlyphsInSelectedGlyphsList) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.SelectGlyphs(Point(30, 10), Point(45, 10));

    // nothing change
    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, c3Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c1Ptr);
    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 35);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 40);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
}

TEST(Document_CutGlyphs,
     DocumentCutGlyphs_WhenCalled_RemoveGlyphsAndSaveThemInSelectedGlyphsList) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.CutGlyphs(Point(36, 10), Point(45, 10));

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_EQ(first, c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_EQ(second, nullptr);

    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);
}

TEST(Document_CutPasteGlyphs,
     DocumentCutPasteGlyphs_WhenCalled_PasteCutGlyphsDueToPosition) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'A');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'A');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.CutGlyphs(Point(30, 10), Point(40, 10));

    document.PasteGlyphs(Point(30, 10));

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_TRUE(first != c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_TRUE(second != c3Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c1Ptr);
    Glyph::GlyphPtr fourth =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            third);
    EXPECT_EQ(fourth, nullptr);

    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 35);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 40);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
}

TEST(Document_SelectPasteGlyphs1,
     DocumentSelectPasteGlyphs_WhenCalled_PasteSelectedGlyphsDueToPosition) {
    Document document(std::make_shared<SimpleCompositor>(
        10, 20, 30, 40, Compositor::LEFT, 100));

    Character c1 = Character(100, 10, 5, 5, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(30, 10, 5, 5, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(35, 10, 5, 5, 'C');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);

    document.SelectGlyphs(Point(30, 10), Point(40, 10));

    document.PasteGlyphs(Point(30, 10));

    Glyph::GlyphPtr first = document.GetFirstPage()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph()
                                ->GetFirstGlyph();
    EXPECT_TRUE(first != c2Ptr);
    Glyph::GlyphPtr second =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            first);
    EXPECT_TRUE(second != c3Ptr);
    Glyph::GlyphPtr third =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            second);
    EXPECT_EQ(third, c2Ptr);
    Glyph::GlyphPtr fourth =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            third);
    EXPECT_EQ(fourth, c3Ptr);
    Glyph::GlyphPtr fifth =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            fourth);
    EXPECT_EQ(fifth, c1Ptr);
    Glyph::GlyphPtr sixth =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph()->GetNextGlyph(
            fifth);
    EXPECT_EQ(sixth, nullptr);

    // check first character params
    EXPECT_EQ(first->GetPosition().x, 30);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 5);
    EXPECT_EQ(first->GetHeight(), 5);
    std::shared_ptr<Character> firstChar =
        std::static_pointer_cast<Character>(first);
    EXPECT_EQ(firstChar->GetChar(), 'B');

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 35);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 5);
    EXPECT_EQ(second->GetHeight(), 5);
    std::shared_ptr<Character> secondChar =
        std::static_pointer_cast<Character>(second);
    EXPECT_EQ(secondChar->GetChar(), 'C');

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 40);
    EXPECT_EQ(third->GetPosition().y, 10);
    EXPECT_EQ(third->GetWidth(), 5);
    EXPECT_EQ(third->GetHeight(), 5);
    std::shared_ptr<Character> thirdChar =
        std::static_pointer_cast<Character>(third);
    EXPECT_EQ(thirdChar->GetChar(), 'B');

    // check fourth character params
    EXPECT_EQ(fourth->GetPosition().x, 45);
    EXPECT_EQ(fourth->GetPosition().y, 10);
    EXPECT_EQ(fourth->GetWidth(), 5);
    EXPECT_EQ(fourth->GetHeight(), 5);
    std::shared_ptr<Character> fourthChar =
        std::static_pointer_cast<Character>(fourth);
    EXPECT_EQ(fourthChar->GetChar(), 'C');

    // check fifth character params
    EXPECT_EQ(fifth->GetPosition().x, 50);
    EXPECT_EQ(fifth->GetPosition().y, 10);
    EXPECT_EQ(fifth->GetWidth(), 5);
    EXPECT_EQ(fifth->GetHeight(), 5);
    std::shared_ptr<Character> fifthChar =
        std::static_pointer_cast<Character>(fifth);
    EXPECT_EQ(fifthChar->GetChar(), 'A');
}

TEST(Document_SelectPasteGlyphs2,
     DocumentSelectPasteGlyphs_WhenCalled_PasteSelectedGlyphsDueToPosition) {
    Document document(std::make_shared<SimpleCompositor>(10, 20, 40, 40,
                                                         Compositor::LEFT, 0));

    Character c1 = Character(40, 10, 210, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(250, 10, 210, 10, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(460, 10, 210, 10, 'C');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    Character c4 = Character(250, 21, 210, 10, 'D');
    Glyph::GlyphPtr c4Ptr = std::make_shared<Character>(c4);
    document.Insert(c1Ptr);
    document.Insert(c2Ptr);
    document.Insert(c3Ptr);
    document.Insert(c4Ptr);

    document.SelectGlyphs(Point(40, 10), Point(250, 30));

    std::list<Glyph::GlyphPtr> pasted = document.PasteGlyphs(Point(250, 21));

    std::cout << "Pasted: " << pasted.size() << std::endl;
    for (auto pastedGlyph : pasted) {
        std::cout << *pastedGlyph << std::endl;
    }

    Glyph::GlyphPtr firstRow =
        document.GetFirstPage()->GetFirstGlyph()->GetFirstGlyph();
    Glyph::GlyphPtr secondRow =
        document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(firstRow);
    Glyph::GlyphPtr thirdRow =
        document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(secondRow);
    Glyph::GlyphPtr fourthRow =
        document.GetFirstPage()->GetFirstGlyph()->GetNextGlyph(thirdRow);

    Glyph::GlyphPtr first = firstRow->GetFirstGlyph();
    EXPECT_EQ(first, c1Ptr);
    Glyph::GlyphPtr second = firstRow->GetNextGlyph(first);
    EXPECT_EQ(second, c2Ptr);
    Glyph::GlyphPtr third = secondRow->GetFirstGlyph();
    EXPECT_EQ(third, c3Ptr);
    Glyph::GlyphPtr fourth = secondRow->GetNextGlyph(third);
    EXPECT_TRUE(fourth != c1Ptr);
    Glyph::GlyphPtr fifth = thirdRow->GetFirstGlyph();
    EXPECT_TRUE(fifth != c2Ptr);
    Glyph::GlyphPtr sixth = thirdRow->GetNextGlyph(fifth);
    EXPECT_TRUE(sixth != c3Ptr);
    Glyph::GlyphPtr seventh = fourthRow->GetFirstGlyph();
    EXPECT_EQ(seventh, c4Ptr);

    // check first character params
    EXPECT_EQ(first->GetPosition().x, 40);
    EXPECT_EQ(first->GetPosition().y, 10);
    EXPECT_EQ(first->GetWidth(), 210);
    EXPECT_EQ(first->GetHeight(), 10);
    std::shared_ptr<Character> firstChar =
        std::static_pointer_cast<Character>(first);
    EXPECT_EQ(firstChar->GetChar(), 'A');

    // check second character params
    EXPECT_EQ(second->GetPosition().x, 250);
    EXPECT_EQ(second->GetPosition().y, 10);
    EXPECT_EQ(second->GetWidth(), 210);
    EXPECT_EQ(second->GetHeight(), 10);
    std::shared_ptr<Character> secondChar =
        std::static_pointer_cast<Character>(second);
    EXPECT_EQ(secondChar->GetChar(), 'B');

    // check third character params
    EXPECT_EQ(third->GetPosition().x, 40);
    EXPECT_EQ(third->GetPosition().y, 20);
    EXPECT_EQ(third->GetWidth(), 210);
    EXPECT_EQ(third->GetHeight(), 10);
    std::shared_ptr<Character> thirdChar =
        std::static_pointer_cast<Character>(third);
    EXPECT_EQ(thirdChar->GetChar(), 'C');

    // check fourth character params
    EXPECT_EQ(fourth->GetPosition().x, 250);
    EXPECT_EQ(fourth->GetPosition().y, 20);
    EXPECT_EQ(fourth->GetWidth(), 210);
    EXPECT_EQ(fourth->GetHeight(), 10);
    std::shared_ptr<Character> fourthChar =
        std::static_pointer_cast<Character>(fourth);
    EXPECT_EQ(fourthChar->GetChar(), 'A');

    // check fifth character params
    EXPECT_EQ(fifth->GetPosition().x, 40);
    EXPECT_EQ(fifth->GetPosition().y, 30);
    EXPECT_EQ(fifth->GetWidth(), 210);
    EXPECT_EQ(fifth->GetHeight(), 10);
    std::shared_ptr<Character> fifthChar =
        std::static_pointer_cast<Character>(fifth);
    EXPECT_EQ(fifthChar->GetChar(), 'B');

    // check sixth character params
    EXPECT_EQ(sixth->GetPosition().x, 250);
    EXPECT_EQ(sixth->GetPosition().y, 30);
    EXPECT_EQ(sixth->GetWidth(), 210);
    EXPECT_EQ(sixth->GetHeight(), 10);
    std::shared_ptr<Character> sixthChar =
        std::static_pointer_cast<Character>(sixth);
    EXPECT_EQ(sixthChar->GetChar(), 'C');

    // check seventh character params
    EXPECT_EQ(seventh->GetPosition().x, 40);
    EXPECT_EQ(seventh->GetPosition().y, 40);
    EXPECT_EQ(seventh->GetWidth(), 210);
    EXPECT_EQ(seventh->GetHeight(), 10);
    std::shared_ptr<Character> seventhChar =
        std::static_pointer_cast<Character>(seventh);
    EXPECT_EQ(seventhChar->GetChar(), 'D');
}

TEST(Tmp_test, Tmp_test) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(10, 0, 10, 10, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    Character c3 = Character(20, 0, 10, 10, 'C');
    Glyph::GlyphPtr c3Ptr = std::make_shared<Character>(c3);
    Character c4 = Character(30, 0, 10, 10, 'D');
    Glyph::GlyphPtr c4Ptr = std::make_shared<Character>(c4);
    d->Insert(c1Ptr);
    d->Insert(c2Ptr);
    d->Insert(c3Ptr);
    d->Insert(c4Ptr);

    d->Remove(c4Ptr);
    d->Insert(c4Ptr);

    // Character c = Character(3, 5, 0, 0, 0);
    // Glyph::GlyphPtr cPtr = std::make_shared<Character>(c);
    // d->Remove(cPtr);

    d->SelectGlyphs(Point(3, 5), Point(43, 5));
    std::list<Glyph::GlyphPtr> pasted = d->PasteGlyphs(Point(43, 5));
    std::cout << pasted.size() << std::endl;
    std::cout << *pasted.front() << std::endl;
    std::cout << *pasted.back() << std::endl;
}

TEST(Document_Cursor1,
     DocumentCursorInTheFirstEmptyRow_WhenCalles_ReturnGlyphNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Row::RowPtr selectedRow = std::dynamic_pointer_cast<Row>(selectedGlyph);
    EXPECT_EQ(selectedRow, d->GetFirstPage()->GetFirstGlyph()->GetFirstGlyph());
}

TEST(Document_Cursor2,
     DocumentCursorAfterInsertedChar_WhenCalles_ReturnGlyphNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    d->Insert(c1Ptr);

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c1Ptr);
}

TEST(Document_Cursor3,
     DocumentCursorAfterInsertedChar_WhenCalles_ReturnGlyphNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    d->Insert(c1Ptr);
    d->Remove(c1Ptr);

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Row::RowPtr selectedRow = std::dynamic_pointer_cast<Row>(selectedGlyph);
    EXPECT_EQ(selectedRow, d->GetFirstPage()->GetFirstGlyph()->GetFirstGlyph());
}

TEST(Document_Cursor4,
     DocumentCursorAfterInsertedChar_WhenCalles_ReturnGlyphNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(13, 5, 10, 10, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    d->Insert(c1Ptr);
    d->Insert(c2Ptr);

    d->Remove(c2Ptr);

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c1Ptr);
}

TEST(Document_MoveCursor1, DocumentMoveCursor_WhenCalles_ChangeSelectedGlyph) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(13, 5, 10, 10, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    d->Insert(c1Ptr);
    d->Insert(c2Ptr);

    d->MoveCursorLeft();

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c1Ptr);

    d->MoveCursorRight();

    selectedGlyph = d->GetSelectedGlyph();
    selectedChar = std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c2Ptr);
}

TEST(Document_MoveCursor2,
     DocumentMoveCursorInTheBeginning_WhenCalles_ChangeSelectedGlyph) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    Character c1 = Character(0, 0, 10, 10, 'A');
    Glyph::GlyphPtr c1Ptr = std::make_shared<Character>(c1);
    Character c2 = Character(13, 5, 10, 10, 'B');
    Glyph::GlyphPtr c2Ptr = std::make_shared<Character>(c2);
    d->Insert(c1Ptr);
    d->Insert(c2Ptr);

    d->MoveCursorLeft();

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c1Ptr);

    d->MoveCursorLeft();

    selectedGlyph = d->GetSelectedGlyph();
    Row::RowPtr selectedRow = std::dynamic_pointer_cast<Row>(selectedGlyph);
    EXPECT_EQ(selectedRow, d->GetFirstPage()->GetFirstGlyph()->GetFirstGlyph());

    d->MoveCursorRight();

    selectedGlyph = d->GetSelectedGlyph();
    selectedChar = std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar, c1Ptr);
}

TEST(Document_InsertCharacter1,
     InsertCharacter_WhenCalles_InsertCharacterNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    d->InsertChar('A');

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar->GetChar(), 'A');
    EXPECT_EQ(selectedChar->GetPosition().x, 3);
    EXPECT_EQ(selectedChar->GetPosition().y, 5);
    EXPECT_EQ(selectedChar->GetWidth(), 1);
    EXPECT_EQ(selectedChar->GetHeight(), 1);
}

TEST(Document_InsertCharacter2,
     InsertTwoCharacter_WhenCalles_InsertTwoCharacterNextToCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    d->InsertChar('A');
    d->InsertChar('B');

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar->GetChar(), 'B');
    EXPECT_EQ(selectedChar->GetPosition().x, 4);
    EXPECT_EQ(selectedChar->GetPosition().y, 5);
    EXPECT_EQ(selectedChar->GetWidth(), 1);
    EXPECT_EQ(selectedChar->GetHeight(), 1);

    d->MoveCursorLeft();

    selectedGlyph = d->GetSelectedGlyph();
    selectedChar = std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar->GetChar(), 'A');
    EXPECT_EQ(selectedChar->GetPosition().x, 3);
    EXPECT_EQ(selectedChar->GetPosition().y, 5);
    EXPECT_EQ(selectedChar->GetWidth(), 1);
    EXPECT_EQ(selectedChar->GetHeight(), 1);
}

TEST(Document_RemoveChar1, RemoveOneChar_WhenCalles_RemoveCharAndMoveCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    d->InsertChar('A');
    d->InsertChar('B');

    d->RemoveChar();

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Character::CharPtr selectedChar =
        std::dynamic_pointer_cast<Character>(selectedGlyph);
    EXPECT_EQ(selectedChar->GetChar(), 'A');
    EXPECT_EQ(selectedChar->GetPosition().x, 3);
    EXPECT_EQ(selectedChar->GetPosition().y, 5);
    EXPECT_EQ(selectedChar->GetWidth(), 1);
    EXPECT_EQ(selectedChar->GetHeight(), 1);
}

TEST(Document_RemoveChar2,
     DocumentRemoveBothCharacters_WhenCalles_RemoveCharactersAndMoveCursor) {
    auto d = std::make_shared<Document>(std::make_shared<SimpleCompositor>());

    d->InsertChar('A');
    d->InsertChar('B');

    d->RemoveChar();
    d->RemoveChar();

    Glyph::GlyphPtr selectedGlyph = d->GetSelectedGlyph();
    Row::RowPtr selectedRow = std::dynamic_pointer_cast<Row>(selectedGlyph);
    EXPECT_EQ(selectedRow, d->GetFirstPage()->GetFirstGlyph()->GetFirstGlyph());
}