#include "jar_voxel_lod.h"
#include "jar_voxel_terrain.h"
#include "mesh_compute_scheduler.h"

JarVoxelLoD::JarVoxelLoD()
    : _automaticUpdate(false), _automaticUpdateDistance(0.0f), _minLodDistance(0.0f), _lodLevelCount(0),
      _lodPadding(0.0f), _autoMeshCoolDown(0.0f), _cameraPosition(0.0f, 0.0f, 0.0f)
{
}

float JarVoxelLoD::get_automatic_update_distance() const
{
    return _automaticUpdateDistance;
}

void JarVoxelLoD::set_automatic_update_distance(float distance)
{
    _automaticUpdateDistance = distance;
}

float JarVoxelLoD::get_min_lod_distance() const
{
    return _minLodDistance;
}

void JarVoxelLoD::set_min_lod_distance(float distance)
{
    _minLodDistance = distance;
}

int JarVoxelLoD::get_lod_level_count() const
{
    return _lodLevelCount;
}

void JarVoxelLoD::set_lod_level_count(int count)
{
    _lodLevelCount = count;
}

bool JarVoxelLoD::get_automatic_update() const
{
    return _automaticUpdate;
}

void JarVoxelLoD::set_automatic_update(bool update)
{
    _automaticUpdate = update;
}

float JarVoxelLoD::get_lod_padding() const
{
    return _lodPadding;
}

void JarVoxelLoD::set_lod_padding(float padding)
{
    _lodPadding = padding;
}

glm::vec3 JarVoxelLoD::get_camera_position() const
{
    return _cameraPosition;
}

void JarVoxelLoD::init()
{
    _lodLevels.resize(_lodLevelCount);
    _lodLevels[0] = _minLodDistance;
    for (int i = 1; i < _lodLevelCount; i++)
    {
        _lodLevels[i] = _lodLevels[i - 1] * 2;
    }

    for (int i = 0; i < _lodLevelCount; i++)
    {
        _lodLevels[i] += _lodPadding;
    }

    std::transform(_lodLevels.begin(), _lodLevels.end(), _lodLevels.begin(), [](float x) { return x * x; });
    _maxChunkSize = std::max(_lodLevelCount - 1, 1) + 4;
}

bool JarVoxelLoD::process(const JarVoxelTerrain &terrain, double delta)
{
    _autoMeshCoolDown -= static_cast<float>(delta);
    if(!get_automatic_update()) return false;
    return update_camera_position(terrain, false);
}

bool JarVoxelLoD::update_camera_position(const JarVoxelTerrain &terrain, const bool force)
{
    if (terrain.is_building())
        return false;
    auto player = terrain.get_player_node();
    if (player == nullptr)
        return false;

    auto p = player->get_global_transform().origin;
    glm::vec3 glmp = {p.x, p.y, p.z};

    if (force || (glm::distance(_cameraPosition, glmp) > _automaticUpdateDistance))
    {
        _cameraPosition = glmp;
        return true;
    }
    return false;
}

int JarVoxelLoD::desired_lod(const VoxelOctreeNode &node, float factor)
{
    auto l = node._size > _maxChunkSize ? 0 : lod_at(node._center);
    return l;
}


inline float JarVoxelLoD::lod_to_grid_size(const int lod) const {
    return (1 << lod) * 2.0f; // times octree scale
}

inline glm::vec3 JarVoxelLoD::snap_to_grid(const glm::vec3 pos, const float grid_size) const {
    return floor(pos / grid_size + 0.5f) * grid_size;
}


inline int JarVoxelLoD::lod_at(const glm::vec3 &position) const {
    glm::vec3 pos = position / 16.0f;
    glm::vec3 cam_pos = _cameraPosition / 16.0f;
    for (int lod = 0; lod < _lodLevels.size(); lod++) {
        float grid_size = lod_to_grid_size(lod);
        glm::vec3 lod_cam_pos = snap_to_grid(cam_pos, grid_size);
        glm::vec3 delta = abs(pos - lod_cam_pos);
        float dist = glm::max(glm::max(delta.x, delta.y), delta.z);
        if (dist < grid_size * (2.0f + 1/16.0f)) {
            return lod;
        }
    }
    return - 1; // Fallback to the largest LOD
}

void JarVoxelLoD::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_automatic_update_distance"), &JarVoxelLoD::get_automatic_update_distance);
    ClassDB::bind_method(D_METHOD("set_automatic_update_distance", "distance"),
                         &JarVoxelLoD::set_automatic_update_distance);
    ClassDB::bind_method(D_METHOD("get_min_lod_distance"), &JarVoxelLoD::get_min_lod_distance);
    ClassDB::bind_method(D_METHOD("set_min_lod_distance", "distance"), &JarVoxelLoD::set_min_lod_distance);
    ClassDB::bind_method(D_METHOD("get_lod_level_count"), &JarVoxelLoD::get_lod_level_count);
    ClassDB::bind_method(D_METHOD("set_lod_level_count", "count"), &JarVoxelLoD::set_lod_level_count);
    ClassDB::bind_method(D_METHOD("get_automatic_update"), &JarVoxelLoD::get_automatic_update);
    ClassDB::bind_method(D_METHOD("set_automatic_update", "update"), &JarVoxelLoD::set_automatic_update);
    ClassDB::bind_method(D_METHOD("get_lod_padding"), &JarVoxelLoD::get_lod_padding);
    ClassDB::bind_method(D_METHOD("set_lod_padding", "padding"), &JarVoxelLoD::set_lod_padding);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "automatic_update_distance"), "set_automatic_update_distance",
                 "get_automatic_update_distance");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "min_lod_distance"), "set_min_lod_distance", "get_min_lod_distance");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_level_count"), "set_lod_level_count", "get_lod_level_count");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "automatic_update"), "set_automatic_update", "get_automatic_update");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lod_padding"), "set_lod_padding", "get_lod_padding");
}
