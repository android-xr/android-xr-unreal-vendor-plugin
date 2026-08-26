/* Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "SpatialPlanesWidget.h"
#include "UnrealSamplesWidgetHelpers.h"

void USpatialPlanesWidget::NativeConstruct()
{
    AddEnumToComboBox(AlignmentComboBox, StaticEnum<ESpatialPlaneAlignment>());
    AddEnumToComboBox(LabelComboBox, StaticEnum<ESpatialPlaneSemanticLabel>());
    if(AlignmentFilter)
    {
        AlignmentFilter->OnCheckStateChanged.RemoveAll(this);
        AlignmentFilter->OnCheckStateChanged.AddUniqueDynamic(this, &USpatialPlanesWidget::AlignmentFilterChangedHandler);
    }
    if(LabelFilter)
    {
        LabelFilter->OnCheckStateChanged.RemoveAll(this);
        LabelFilter->OnCheckStateChanged.AddUniqueDynamic(this, &USpatialPlanesWidget::LabelFilterChangedHandler);
    }
    if(RenderBoundsCheckbox)
    {
        RenderBoundsCheckbox->OnCheckStateChanged.RemoveAll(this);
        RenderBoundsCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &USpatialPlanesWidget::BoundsRenderChangedHandler);
    }
    if(RenderPolygonsCheckbox)
    {
        RenderPolygonsCheckbox->OnCheckStateChanged.RemoveAll(this);
        RenderPolygonsCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &USpatialPlanesWidget::PolygonRenderChangedHandler);
    }
    if(RenderMeshCheckbox)
    {
        RenderMeshCheckbox->OnCheckStateChanged.RemoveAll(this);
        RenderMeshCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &USpatialPlanesWidget::MeshRenderChangedHandler);
    }

    Super::NativeConstruct();
}

void USpatialPlanesWidget::LabelFilterChangedHandler(bool bEnableFilter)
{
    RenderFilter.SemanticLabel = GetValueFromComboBox<ESpatialPlaneSemanticLabel>(LabelComboBox);
    RenderFilter.bSemanticFilter = bEnableFilter;
    NotifyRenderFilterChanged();
}

void USpatialPlanesWidget::AlignmentFilterChangedHandler(bool bEnableFilter)
{
    RenderFilter.bAlignmentFilter = bEnableFilter;
    RenderFilter.Alignment = GetValueFromComboBox<ESpatialPlaneAlignment>(AlignmentComboBox);
    NotifyRenderFilterChanged();
}

void USpatialPlanesWidget::BoundsRenderChangedHandler(bool bEnableFilter)
{
    RenderFilter.bShouldRenderBounds = bEnableFilter;
    NotifyRenderFilterChanged();
}

void USpatialPlanesWidget::MeshRenderChangedHandler(bool bEnableFilter)
{
    RenderFilter.bShouldRenderMesh = bEnableFilter;
    NotifyRenderFilterChanged();
}

void USpatialPlanesWidget::PolygonRenderChangedHandler(bool bEnableFilter)
{
    RenderFilter.bShouldRenderPolygons = bEnableFilter;
    NotifyRenderFilterChanged();
}

void USpatialPlanesWidget::NotifyRenderFilterChanged()
{
    OnRenderFilterChanged.Broadcast(RenderFilter);
}
