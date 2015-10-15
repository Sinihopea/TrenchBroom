/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_Polyhedron_Misc_h
#define TrenchBroom_Polyhedron_Misc_h

#include <map>

template <typename T, typename FP>
const typename Polyhedron<T,FP>::V& Polyhedron<T,FP>::GetVertexPosition::operator()(const Vertex* vertex) const {
    return vertex->position();
}

template <typename T, typename FP>
const typename Polyhedron<T,FP>::V& Polyhedron<T,FP>::GetVertexPosition::operator()(const HalfEdge* halfEdge) const {
    return halfEdge->origin()->position();
}

template <typename T, typename FP>
Polyhedron<T,FP>::Callback::~Callback() {}

template <typename T, typename FP>
Plane<T,3> Polyhedron<T,FP>::Callback::plane(const Face* face) const {
    const HalfEdgeList& boundary = face->boundary();
    assert(boundary.size() >= 3);
    
    const HalfEdge* e1 = boundary.front();
    const HalfEdge* e2 = e1->next();
    const HalfEdge* e3 = e2->next();
    
    const V& p1 = e1->origin()->position();
    const V& p2 = e2->origin()->position();
    const V& p3 = e3->origin()->position();

    Plane<T,3> plane;
    assertResult(setPlanePoints(plane, p2, p1, p3));
    return plane;
}

template <typename T, typename FP>
void Polyhedron<T,FP>::Callback::faceWasCreated(Face* face) {}

template <typename T, typename FP>
void Polyhedron<T,FP>::Callback::faceWillBeDeleted(Face* face) {}

template <typename T, typename FP>
void Polyhedron<T,FP>::Callback::faceDidChange(Face* face) {}

template <typename T, typename FP>
void Polyhedron<T,FP>::Callback::faceWasSplit(Face* original, Face* clone) {}

template <typename T, typename FP>
void Polyhedron<T,FP>::Callback::facesWillBeMerged(Face* remaining, Face* toDelete) {}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron() {
    updateBounds();
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const V& p1, const V& p2, const V& p3, const V& p4) {
    Callback c;
    addPoints(p1, p2, p3, p4, c);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const V& p1, const V& p2, const V& p3, const V& p4, Callback& callback) {
    addPoints(p1, p2, p3, p4, callback);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const BBox<T,3>& bounds) {
    Callback c;
    setBounds(bounds, c);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const BBox<T,3>& bounds, Callback& callback) {
    setBounds(bounds, callback);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const typename V::List& positions) {
    Callback c;
    addPoints(positions.begin(), positions.end(), c);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const typename V::List& positions, Callback& callback) {
    addPoints(positions.begin(), positions.end(), callback);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const typename V::Set& positions) {
    Callback c;
    addPoints(positions.begin(), positions.end(), c);
}

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const typename V::Set& positions, Callback& callback) {
    addPoints(positions.begin(), positions.end(), callback);
}

template <typename T, typename FP>
void Polyhedron<T,FP>::addPoints(const V& p1, const V& p2, const V& p3, const V& p4, Callback& callback) {
    addPoint(p1, callback);
    addPoint(p2, callback);
    addPoint(p3, callback);
    addPoint(p4, callback);
}

template <typename T, typename FP>
void Polyhedron<T,FP>::setBounds(const BBox<T,3>& bounds, Callback& callback) {
    if (bounds.min == bounds.max) {
        addPoint(bounds.min);
        return;
    }
    
    // Explicitely create the polyhedron for better performance when building brushes.
    
    const V p1(bounds.min.x(), bounds.min.y(), bounds.min.z());
    const V p2(bounds.min.x(), bounds.min.y(), bounds.max.z());
    const V p3(bounds.min.x(), bounds.max.y(), bounds.min.z());
    const V p4(bounds.min.x(), bounds.max.y(), bounds.max.z());
    const V p5(bounds.max.x(), bounds.min.y(), bounds.min.z());
    const V p6(bounds.max.x(), bounds.min.y(), bounds.max.z());
    const V p7(bounds.max.x(), bounds.max.y(), bounds.min.z());
    const V p8(bounds.max.x(), bounds.max.y(), bounds.max.z());
    
    Vertex* v1 = new Vertex(p1);
    Vertex* v2 = new Vertex(p2);
    Vertex* v3 = new Vertex(p3);
    Vertex* v4 = new Vertex(p4);
    Vertex* v5 = new Vertex(p5);
    Vertex* v6 = new Vertex(p6);
    Vertex* v7 = new Vertex(p7);
    Vertex* v8 = new Vertex(p8);

    m_vertices.append(v1, 1);
    m_vertices.append(v2, 1);
    m_vertices.append(v3, 1);
    m_vertices.append(v4, 1);
    m_vertices.append(v5, 1);
    m_vertices.append(v6, 1);
    m_vertices.append(v7, 1);
    m_vertices.append(v8, 1);
    
    // Front face
    HalfEdge* f1h1 = new HalfEdge(v1);
    HalfEdge* f1h2 = new HalfEdge(v5);
    HalfEdge* f1h3 = new HalfEdge(v6);
    HalfEdge* f1h4 = new HalfEdge(v2);
    HalfEdgeList f1b;
    f1b.append(f1h1, 1);
    f1b.append(f1h2, 1);
    f1b.append(f1h3, 1);
    f1b.append(f1h4, 1);
    m_faces.append(new Face(f1b), 1);
    
    // Left face
    HalfEdge* f2h1 = new HalfEdge(v1);
    HalfEdge* f2h2 = new HalfEdge(v2);
    HalfEdge* f2h3 = new HalfEdge(v4);
    HalfEdge* f2h4 = new HalfEdge(v3);
    HalfEdgeList f2b;
    f2b.append(f2h1, 1);
    f2b.append(f2h2, 1);
    f2b.append(f2h3, 1);
    f2b.append(f2h4, 1);
    m_faces.append(new Face(f2b), 1);
    
    // Bottom face
    HalfEdge* f3h1 = new HalfEdge(v1);
    HalfEdge* f3h2 = new HalfEdge(v3);
    HalfEdge* f3h3 = new HalfEdge(v7);
    HalfEdge* f3h4 = new HalfEdge(v5);
    HalfEdgeList f3b;
    f3b.append(f3h1, 1);
    f3b.append(f3h2, 1);
    f3b.append(f3h3, 1);
    f3b.append(f3h4, 1);
    m_faces.append(new Face(f3b), 1);
    
    // Top face
    HalfEdge* f4h1 = new HalfEdge(v2);
    HalfEdge* f4h2 = new HalfEdge(v6);
    HalfEdge* f4h3 = new HalfEdge(v8);
    HalfEdge* f4h4 = new HalfEdge(v4);
    HalfEdgeList f4b;
    f4b.append(f4h1, 1);
    f4b.append(f4h2, 1);
    f4b.append(f4h3, 1);
    f4b.append(f4h4, 1);
    m_faces.append(new Face(f4b), 1);
    
    // Back face
    HalfEdge* f5h1 = new HalfEdge(v3);
    HalfEdge* f5h2 = new HalfEdge(v4);
    HalfEdge* f5h3 = new HalfEdge(v8);
    HalfEdge* f5h4 = new HalfEdge(v7);
    HalfEdgeList f5b;
    f5b.append(f5h1, 1);
    f5b.append(f5h2, 1);
    f5b.append(f5h3, 1);
    f5b.append(f5h4, 1);
    m_faces.append(new Face(f5b), 1);
    
    // Right face
    HalfEdge* f6h1 = new HalfEdge(v5);
    HalfEdge* f6h2 = new HalfEdge(v7);
    HalfEdge* f6h3 = new HalfEdge(v8);
    HalfEdge* f6h4 = new HalfEdge(v6);
    HalfEdgeList f6b;
    f6b.append(f6h1, 1);
    f6b.append(f6h2, 1);
    f6b.append(f6h3, 1);
    f6b.append(f6h4, 1);
    m_faces.append(new Face(f6b), 1);
    
    m_edges.append(new Edge(f1h4, f2h1), 1); // v1, v2
    m_edges.append(new Edge(f2h4, f3h1), 1); // v1, v3
    m_edges.append(new Edge(f1h1, f3h4), 1); // v1, v5
    m_edges.append(new Edge(f2h2, f4h4), 1); // v2, v4
    m_edges.append(new Edge(f4h1, f1h3), 1); // v2, v6
    m_edges.append(new Edge(f2h3, f5h1), 1); // v3, v4
    m_edges.append(new Edge(f3h2, f5h4), 1); // v3, v7
    m_edges.append(new Edge(f4h3, f5h2), 1); // v4, v8
    m_edges.append(new Edge(f1h2, f6h4), 1); // v5, v6
    m_edges.append(new Edge(f6h1, f3h3), 1); // v5, v7
    m_edges.append(new Edge(f6h3, f4h2), 1); // v6, v8
    m_edges.append(new Edge(f6h2, f5h3), 1); // v7, v8
}

template <typename T, typename FP>
class Polyhedron<T,FP>::Copy {
private:
    typedef std::map<const Vertex*, Vertex*> VertexMap;
    typedef typename VertexMap::value_type VertexMapEntry;

    typedef std::map<const HalfEdge*, HalfEdge*> HalfEdgeMap;
    typedef typename HalfEdgeMap::value_type HalfEdgeMapEntry;

    VertexMap m_vertexMap;
    HalfEdgeMap m_halfEdgeMap;
    
    VertexList m_vertices;
    EdgeList m_edges;
    FaceList m_faces;
    Polyhedron& m_destination;
public:
    Copy(const FaceList& originalFaces, const EdgeList& originalEdges, Polyhedron& destination) :
    m_destination(destination) {
        copyFaces(originalFaces);
        copyEdges(originalEdges);
        swapContents();
    }
private:
    void copyFaces(const FaceList& originalFaces) {
        typename FaceList::const_iterator fIt, fEnd;
        for (fIt = originalFaces.begin(), fEnd = originalFaces.end(); fIt != fEnd; ++fIt) {
            const Face* originalFace = *fIt;
            copyFace(originalFace);
        }
    }
    
    void copyFace(const Face* originalFace) {
        HalfEdgeList myBoundary;

        typename HalfEdgeList::const_iterator hIt, hEnd;
        for (hIt = originalFace->m_boundary.begin(), hEnd = originalFace->m_boundary.end(); hIt != hEnd; ++hIt) {
            const HalfEdge* originalHalfEdge = *hIt;
            myBoundary.append(copyHalfEdge(originalHalfEdge), 1);
        }
        
        Face* copy = new Face(myBoundary);
        m_faces.append(copy, 1);
    }
    
    HalfEdge* copyHalfEdge(const HalfEdge* original) {
        const Vertex* originalOrigin = original->origin();
        
        Vertex* myOrigin = findOrCopyVertex(originalOrigin);
        HalfEdge* copy = new HalfEdge(myOrigin);
        m_halfEdgeMap.insert(std::make_pair(original, copy));
        return copy;
    }

    Vertex* findOrCopyVertex(const Vertex* original) {
        typedef std::pair<bool, typename VertexMap::iterator> InsertPos;
        
        InsertPos insertPos = MapUtils::findInsertPos(m_vertexMap, original);
        if (!insertPos.first) {
            Vertex* copy = new Vertex(original->position());
            m_vertices.append(copy, 1);
            m_vertexMap.insert(insertPos.second, std::make_pair(original, copy));
            return copy;
        }
        return insertPos.second->second;
    }
    
    void copyEdges(const EdgeList& originalEdges) {
        typename EdgeList::const_iterator eIt, eEnd;
        for (eIt = originalEdges.begin(), eEnd = originalEdges.end(); eIt != eEnd; ++eIt) {
            const Edge* originalEdge = *eIt;
            Edge* copy = copyEdge(originalEdge);
            m_edges.append(copy, 1);
        }
    }
    
    Edge* copyEdge(const Edge* original) const {
        HalfEdge* myFirst = findHalfEdge(original->firstEdge());
        if (!original->fullySpecified())
            return new Edge(myFirst);

        HalfEdge* mySecond = findHalfEdge(original->secondEdge());
        return new Edge(myFirst, mySecond);
    }
    
    HalfEdge* findHalfEdge(const HalfEdge* original) const {
        HalfEdge* result = MapUtils::find(m_halfEdgeMap, original, static_cast<HalfEdge*>(NULL));
        assert(result != NULL);
        return result;
    }

    void swapContents() {
        using std::swap;
        swap(m_vertices, m_destination.m_vertices);
        swap(m_edges, m_destination.m_edges);
        swap(m_faces, m_destination.m_faces);
        m_destination.updateBounds();
    }
};

template <typename T, typename FP>
Polyhedron<T,FP>::Polyhedron(const Polyhedron<T,FP>& other) {
    Copy copy(other.faces(), other.edges(), *this);
}

template <typename T, typename FP>
Polyhedron<T,FP>::~Polyhedron() {
    clear();
}

template <typename T, typename FP>
Polyhedron<T,FP>& Polyhedron<T,FP>::operator=(Polyhedron<T,FP> other) {
    swap(*this, other);
    return *this;
}

template <typename T, typename FP>
size_t Polyhedron<T,FP>::vertexCount() const {
    return m_vertices.size();
}

template <typename T, typename FP>
const typename Polyhedron<T,FP>::VertexList& Polyhedron<T,FP>::vertices() const {
    return m_vertices;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::hasVertex(const V& position, const T epsilon) const {
    return findVertexByPosition(position, epsilon) != NULL;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::hasVertices(const typename V::List& positions, const T epsilon) const {
    typename V::List::const_iterator it, end;
    for (it = positions.begin(), end = positions.end(); it != end; ++it) {
        if (!hasVertex(*it, epsilon))
            return false;
    }
    return true;
}

template <typename T, typename FP>
void Polyhedron<T,FP>::printVertices() const {
    const Vertex* firstVertex = m_vertices.front();
    const Vertex* currentVertex = firstVertex;
    do {
        std::cout << currentVertex->position().asString() << std::endl;
        currentVertex = currentVertex->next();
    } while (currentVertex != firstVertex);
}


template <typename T, typename FP>
size_t Polyhedron<T,FP>::edgeCount() const {
    return m_edges.size();
}

template <typename T, typename FP>
const typename Polyhedron<T,FP>::EdgeList& Polyhedron<T,FP>::edges() const {
    return m_edges;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::hasEdge(const V& pos1, const V& pos2, const T epsilon) const {
    return findEdgeByPositions(pos1, pos2, epsilon) != NULL;
}

template <typename T, typename FP>
size_t Polyhedron<T,FP>::faceCount() const {
    return m_faces.size();
}

template <typename T, typename FP>
const typename Polyhedron<T,FP>::FaceList& Polyhedron<T,FP>::faces() const {
    return m_faces;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::hasFace(const typename V::List& positions, const T epsilon) const {
    return findFaceByPositions(positions, epsilon) != NULL;
}

template <typename T, typename FP>
const BBox<T,3>& Polyhedron<T,FP>::bounds() const {
    return m_bounds;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::empty() const {
    return vertexCount() == 0;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::point() const {
    return vertexCount() == 1;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::edge() const {
    return vertexCount() == 2;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::polygon() const {
    return faceCount() == 1;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::polyhedron() const {
    return faceCount() > 3;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::closed() const {
    return vertexCount() + faceCount() == edgeCount() + 2;
}

template <typename T, typename FP>
void Polyhedron<T,FP>::clear() {
    m_faces.clear();
    m_edges.clear();
    m_vertices.clear();
}

template <typename T, typename FP>
struct Polyhedron<T,FP>::FaceHit {
    Face* face;
    T distance;
    
    FaceHit(Face* i_face, const T i_distance) : face(i_face), distance(i_distance) {}
    FaceHit() : face(NULL), distance(Math::nan<T>()) {}
    bool isMatch() const { return face != NULL; }
};

template <typename T, typename FP>
typename Polyhedron<T,FP>::FaceHit Polyhedron<T,FP>::pickFace(const Ray<T,3>& ray) const {
    const Math::Side side = polygon() ? Math::Side_Both : Math::Side_Front;
    typename FaceList::const_iterator it, end;
    for (it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
        Face* face = *it;
        const T distance = face->intersectWithRay(ray, side);
        if (!Math::isnan(distance))
            return FaceHit(face, distance);
    }
    return FaceHit();
}

template <typename T, typename FP>
typename Polyhedron<T,FP>::Vertex* Polyhedron<T,FP>::findVertexByPosition(const V& position, const T epsilon) const {
    typename VertexList::const_iterator it, end;
    for (it = m_vertices.begin(), end = m_vertices.end(); it != end; ++it) {
        Vertex* vertex = *it;
        if (position.equals(vertex->position(), epsilon))
            return vertex;
    }
    return NULL;
}

template <typename T, typename FP>
typename Polyhedron<T,FP>::Edge* Polyhedron<T,FP>::findEdgeByPositions(const V& pos1, const V& pos2, const T epsilon) const {
    typename EdgeList::const_iterator it, end;
    for (it = m_edges.begin(), end = m_edges.end(); it != end; ++it) {
        Edge* edge = *it;
        if (edge->hasPositions(pos1, pos2, epsilon))
            return edge;
    }
    return NULL;
}

template <typename T, typename FP>
typename Polyhedron<T,FP>::Face* Polyhedron<T,FP>::findFaceByPositions(const typename V::List& positions, const T epsilon) const {
    typename FaceList::const_iterator it, end;
    for (it = m_faces.begin(), end = m_faces.end(); it != end; ++it) {
        Face* face = *it;
        if (face->hasVertexPositions(positions, epsilon))
            return face;
    }
    return NULL;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::checkInvariant() const {
    if (!checkConvex())
        return false;
    if (polyhedron() && !checkClosed())
        return false;
    if (polyhedron() && !checkNoDegenerateFaces())
        return false;
    /* This check leads to false positive with almost coplanar faces.
    if (polyhedron() && !checkNoCoplanarFaces())
        return false;
     */
    return true;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::checkConvex() const {
    typename FaceList::const_iterator fIt, fEnd;
    for (fIt = m_faces.begin(), fEnd = m_faces.end(); fIt != fEnd; ++fIt) {
        const Face* face = *fIt;
        typename VertexList::const_iterator vIt, vEnd;
        for (vIt = m_vertices.begin(), vEnd = m_vertices.end(); vIt != vEnd; ++vIt) {
            const Vertex* vertex = *vIt;
            if (face->pointStatus(vertex->position()) == Math::PointStatus::PSAbove)
                return false;
        }
    }
    return true;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::checkClosed() const {
    typename EdgeList::const_iterator eIt, eEnd;
    for (eIt = m_edges.begin(), eEnd = m_edges.end(); eIt != eEnd; ++eIt) {
        const Edge* edge = *eIt;
        if (!edge->fullySpecified())
            return false;
        
        const Face* firstFace = edge->firstFace();
        const Face* secondFace = edge->secondFace();
        
        if (!m_faces.contains(firstFace))
            return false;
        if (!m_faces.contains(secondFace))
            return false;
    }
    return true;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::checkNoCoplanarFaces() const {
    typename EdgeList::const_iterator eIt, eEnd;
    for (eIt = m_edges.begin(), eEnd = m_edges.end(); eIt != eEnd; ++eIt) {
        const Edge* edge = *eIt;
        const Face* firstFace = edge->firstFace();
        const Face* secondFace = edge->secondFace();
        
        if (firstFace == secondFace)
            return false;
        if (firstFace->coplanar(secondFace))
            return false;
    }
    return true;
}

template <typename T, typename FP>
bool Polyhedron<T,FP>::checkNoDegenerateFaces() const {
    typename FaceList::const_iterator fIt, fEnd;
    for (fIt = m_faces.begin(), fEnd = m_faces.end(); fIt != fEnd; ++fIt) {
        const Face* face = *fIt;
        if (face->vertexCount() < 3)
            return false;
        
        const HalfEdgeList& boundary = face->boundary();
        typename HalfEdgeList::const_iterator hIt, hEnd;
        for (hIt = boundary.begin(), hEnd = boundary.end(); hIt != hEnd; ++hIt) {
            const HalfEdge* halfEdge = *hIt;
            const Edge* edge = halfEdge->edge();
            
            if (edge == NULL || !edge->fullySpecified())
                return false;
        }
    }
    return true;
}

template <typename T, typename FP>
void Polyhedron<T,FP>::updateBounds() {
    if (m_vertices.size() == 0) {
        m_bounds.min = m_bounds.max = Vec<T,3>::NaN;
    } else {
        Vertex* first = m_vertices.front();
        Vertex* current = first;
        m_bounds.min = m_bounds.max = current->position();
        
        current = current->next();
        while (current != first) {
            m_bounds.mergeWith(current->position());
            current = current->next();
        }
    }
}

#endif
