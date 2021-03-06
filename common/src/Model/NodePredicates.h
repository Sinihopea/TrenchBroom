/*
 Copyright (C) 2010-2017 Kristian Duske

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

#ifndef TrenchBroom_NodePredicates
#define TrenchBroom_NodePredicates

#undef True
#undef False

namespace TrenchBroom {
    namespace Model {
        class BrushNode;
        class EntityNode;
        class GroupNode;
        class LayerNode;
        class Node;
        class Object;
        class WorldNode;

        namespace NodePredicates {
            struct True {
                bool operator()(const Node* node) const;
            };

            struct False {
                bool operator()(const Node* node) const;
            };

            template <typename P>
            class Id {
            private:
                P m_p;
            public:
                explicit Id(const P& p) :
                m_p(p) {}

                bool operator()(const WorldNode* world) const   { return m_p(world);  }
                bool operator()(WorldNode* world) const         { return m_p(world);  }
                bool operator()(const LayerNode* layer) const   { return m_p(layer);  }
                bool operator()(LayerNode* layer) const         { return m_p(layer);  }
                bool operator()(const GroupNode* group) const   { return m_p(group);  }
                bool operator()(GroupNode* group) const         { return m_p(group);  }
                bool operator()(const EntityNode* entity) const { return m_p(entity); }
                bool operator()(EntityNode* entity) const       { return m_p(entity); }
                bool operator()(const BrushNode* brush) const   { return m_p(brush);  }
                bool operator()(BrushNode* brush) const         { return m_p(brush);  }
            };

            template <typename P>
            class Not {
            private:
                P m_p;
            public:
                explicit Not(const P& p) :
                m_p(p) {}

                bool operator()(const WorldNode* world) const   { return !m_p(world);  }
                bool operator()(WorldNode* world) const         { return !m_p(world);  }
                bool operator()(const LayerNode* layer) const   { return !m_p(layer);  }
                bool operator()(LayerNode* layer) const         { return !m_p(layer);  }
                bool operator()(const GroupNode* group) const   { return !m_p(group);  }
                bool operator()(GroupNode* group) const         { return !m_p(group);  }
                bool operator()(const EntityNode* entity) const { return !m_p(entity); }
                bool operator()(EntityNode* entity) const       { return !m_p(entity); }
                bool operator()(const BrushNode* brush) const   { return !m_p(brush);  }
                bool operator()(BrushNode* brush) const         { return !m_p(brush);  }
            };

            template <typename P1, typename P2>
            class And {
            private:
                P1 m_p1;
                P2 m_p2;
            public:
                And(const P1& p1, const P2& p2) :
                m_p1(p1),
                m_p2(p2) {}

                bool operator()(const WorldNode* world) const   { return m_p1(world)  && m_p2(world);  }
                bool operator()(WorldNode* world) const         { return m_p1(world)  && m_p2(world);  }
                bool operator()(const LayerNode* layer) const   { return m_p1(layer)  && m_p2(layer);  }
                bool operator()(LayerNode* layer) const         { return m_p1(layer)  && m_p2(layer);  }
                bool operator()(const GroupNode* group) const   { return m_p1(group)  && m_p2(group);  }
                bool operator()(GroupNode* group) const         { return m_p1(group)  && m_p2(group);  }
                bool operator()(const EntityNode* entity) const { return m_p1(entity) && m_p2(entity); }
                bool operator()(EntityNode* entity) const       { return m_p1(entity) && m_p2(entity); }
                bool operator()(const BrushNode* brush) const   { return m_p1(brush)  && m_p2(brush);  }
                bool operator()(BrushNode* brush) const         { return m_p1(brush)  && m_p2(brush);  }
            };

            template <typename P1, typename P2>
            class Or {
            private:
                P1 m_p1;
                P2 m_p2;
            public:
                Or(const P1& p1, const P2& p2) :
                m_p1(p1),
                m_p2(p2) {}

                bool operator()(const WorldNode* world) const   { return m_p1(world)  || m_p2(world);  }
                bool operator()(WorldNode* world) const         { return m_p1(world)  || m_p2(world);  }
                bool operator()(const LayerNode* layer) const   { return m_p1(layer)  || m_p2(layer);  }
                bool operator()(LayerNode* layer) const         { return m_p1(layer)  || m_p2(layer);  }
                bool operator()(const GroupNode* group) const   { return m_p1(group)  || m_p2(group);  }
                bool operator()(GroupNode* group) const         { return m_p1(group)  || m_p2(group);  }
                bool operator()(const EntityNode* entity) const { return m_p1(entity) || m_p2(entity); }
                bool operator()(EntityNode* entity) const       { return m_p1(entity) || m_p2(entity); }
                bool operator()(const BrushNode* brush) const   { return m_p1(brush)  || m_p2(brush);  }
                bool operator()(BrushNode* brush) const         { return m_p1(brush)  || m_p2(brush);  }
            };

            class EqualsNode {
            private:
                const Node* m_node;
            public:
                explicit EqualsNode(const Node* node) :
                m_node(node) {}

                bool operator()(const WorldNode* world) const;
                bool operator()(WorldNode* world) const;
                bool operator()(const LayerNode* layer) const;
                bool operator()(LayerNode* layer) const;
                bool operator()(const GroupNode* group) const;
                bool operator()(GroupNode* group) const;
                bool operator()(const EntityNode* entity) const;
                bool operator()(EntityNode* entity) const;
                bool operator()(const BrushNode* brush) const;
                bool operator()(BrushNode* brush) const;
            };

            class EqualsObject {
            private:
                const Object* m_object;
            public:
                explicit EqualsObject(const Object* object) :
                m_object(object) {}

                bool operator()(const WorldNode* world) const;
                bool operator()(WorldNode* world) const;
                bool operator()(const LayerNode* layer) const;
                bool operator()(LayerNode* layer) const;
                bool operator()(const GroupNode* group) const;
                bool operator()(GroupNode* group) const;
                bool operator()(const EntityNode* entity) const;
                bool operator()(EntityNode* entity) const;
                bool operator()(const BrushNode* brush) const;
                bool operator()(BrushNode* brush) const;
            };
        }
    }
}

#endif /* defined(TrenchBroom_NodePredicates) */
