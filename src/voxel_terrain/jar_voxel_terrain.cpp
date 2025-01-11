#include "jar_voxel_terrain.h"
#include "jar_plane_sdf.h"
#include "jar_sphere_sdf.h"
#include "modify_settings.h"

void JarVoxelTerrain::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_octree_scale"), &JarVoxelTerrain::get_octree_scale);
    ClassDB::bind_method(D_METHOD("set_octree_scale", "value"), &JarVoxelTerrain::set_octree_scale);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "octree_scale"), "set_octree_scale", "get_octree_scale");

    ClassDB::bind_method(D_METHOD("get_size"), &JarVoxelTerrain::get_size);
    ClassDB::bind_method(D_METHOD("set_size", "value"), &JarVoxelTerrain::set_size);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "size"), "set_size", "get_size");

    ClassDB::bind_method(D_METHOD("get_min_chunk_size"), &JarVoxelTerrain::get_min_chunk_size);
    ClassDB::bind_method(D_METHOD("set_min_chunk_size", "value"), &JarVoxelTerrain::set_min_chunk_size);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "min_chunk_size"), "set_min_chunk_size", "get_min_chunk_size");

    ClassDB::bind_method(D_METHOD("get_update_lod_automatically"), &JarVoxelTerrain::get_update_lod_automatically);
    ClassDB::bind_method(D_METHOD("set_update_lod_automatically", "value"),
                         &JarVoxelTerrain::set_update_lod_automatically);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "update_lod_automatically"), "set_update_lod_automatically",
                 "get_update_lod_automatically");

    ClassDB::bind_method(D_METHOD("get_automatic_update_distance"), &JarVoxelTerrain::get_automatic_update_distance);
    ClassDB::bind_method(D_METHOD("set_automatic_update_distance", "value"),
                         &JarVoxelTerrain::set_automatic_update_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "automatic_update_distance"), "set_automatic_update_distance",
                 "get_automatic_update_distance");

    ClassDB::bind_method(D_METHOD("get_lod_count"), &JarVoxelTerrain::get_lod_count);
    ClassDB::bind_method(D_METHOD("set_lod_count", "value"), &JarVoxelTerrain::set_lod_count);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "lod_count"), "set_lod_count", "get_lod_count");

    ClassDB::bind_method(D_METHOD("get_lod_distance"), &JarVoxelTerrain::get_lod_distance);
    ClassDB::bind_method(D_METHOD("set_lod_distance", "value"), &JarVoxelTerrain::set_lod_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lod_distance"), "set_lod_distance", "get_lod_distance");

    ClassDB::bind_method(D_METHOD("get_lod_padding"), &JarVoxelTerrain::get_lod_padding);
    ClassDB::bind_method(D_METHOD("set_lod_padding", "value"), &JarVoxelTerrain::set_lod_padding);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lod_padding"), "set_lod_padding", "get_lod_padding");

    ClassDB::bind_method(D_METHOD("get_chunk_scene"), &JarVoxelTerrain::get_chunk_scene);
    ClassDB::bind_method(D_METHOD("set_chunk_scene", "value"), &JarVoxelTerrain::set_chunk_scene);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "chunk_scene", PROPERTY_HINT_RESOURCE_TYPE, "PackedScene"),
                 "set_chunk_scene", "get_chunk_scene");

    ClassDB::bind_method(D_METHOD("get_sdf"), &JarVoxelTerrain::get_sdf);
    ClassDB::bind_method(D_METHOD("set_sdf", "sdf"), &JarVoxelTerrain::set_sdf);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sdf", PROPERTY_HINT_RESOURCE_TYPE, "JarSignedDistanceField"), "set_sdf",
                 "get_sdf");

    ClassDB::bind_method(D_METHOD("sphere_edit_terrain", "position", "radius", "change"),
                         &JarVoxelTerrain::sphere_edit_terrain);
}

JarVoxelTerrain::JarVoxelTerrain()
    : _octreeScale(1.0f), _size(14), _updateLoDAutomatically(true), _automaticUpdateDistance(64.0f), _lodCount(8),
      _lodDistance(128.0f), _lodPadding(0.0f)
{
    _chunkSize = (1 << _minChunkSize);
}

bool JarVoxelTerrain::is_building() const
{
    return _isBuilding;
}

VoxelLoD *JarVoxelTerrain::get_lod() const
{
    return _levelOfDetail;
}

Ref<JarSignedDistanceField> JarVoxelTerrain::get_sdf() const
{
    return _sdf;
}
void JarVoxelTerrain::set_sdf(const Ref<JarSignedDistanceField> &sdf)
{
    _sdf = sdf;
}

MeshComputeScheduler *JarVoxelTerrain::get_mesh_scheduler() const
{
    return _meshComputeScheduler;
}

float JarVoxelTerrain::get_octree_scale() const
{
    return _octreeScale;
}

void JarVoxelTerrain::set_octree_scale(float value)
{
    _octreeScale = value;
}

int JarVoxelTerrain::get_size() const
{
    return _size;
}

void JarVoxelTerrain::set_size(int value)
{
    _size = value;
}

int JarVoxelTerrain::get_min_chunk_size() const
{
    return _minChunkSize;
}

void JarVoxelTerrain::set_min_chunk_size(int value)
{
    _minChunkSize = value;
}

int JarVoxelTerrain::get_chunk_size() const
{
    return _chunkSize;
}

bool JarVoxelTerrain::get_update_lod_automatically() const
{
    return _updateLoDAutomatically;
}

void JarVoxelTerrain::set_update_lod_automatically(bool value)
{
    _updateLoDAutomatically = value;
}

float JarVoxelTerrain::get_automatic_update_distance() const
{
    return _automaticUpdateDistance;
}

void JarVoxelTerrain::set_automatic_update_distance(float value)
{
    _automaticUpdateDistance = value;
}

int JarVoxelTerrain::get_lod_count() const
{
    return _lodCount;
}

void JarVoxelTerrain::set_lod_count(int value)
{
    _lodCount = value;
}

float JarVoxelTerrain::get_lod_distance() const
{
    return _lodDistance;
}

void JarVoxelTerrain::set_lod_distance(float value)
{
    _lodDistance = value;
}

float JarVoxelTerrain::get_lod_padding() const
{
    return _lodPadding;
}

void JarVoxelTerrain::set_lod_padding(float value)
{
    _lodPadding = value;
}

Ref<PackedScene> JarVoxelTerrain::get_chunk_scene() const
{
    return _chunkScene;
}

void JarVoxelTerrain::set_chunk_scene(const Ref<PackedScene> &value)
{
    _chunkScene = value;
}

void JarVoxelTerrain::_notification(int p_what)
{
    if (godot::Engine::get_singleton()->is_editor_hint())
    {
        return;
    }
    switch (p_what)
    {
    case NOTIFICATION_ENTER_TREE: {
        if (!Engine::get_singleton()->is_editor_hint())
        {
            set_process_internal(true);
        }
        else
        {
            set_process_internal(false);
        }
        break;
    }
    case NOTIFICATION_READY: {
        initialize();
        break;
    }
    case NOTIFICATION_EXIT_TREE: {
        set_process_internal(false);
        break;
    }
    case NOTIFICATION_INTERNAL_PROCESS: {
        process();
        break;
    }
    }
}

void JarVoxelTerrain::initialize()
{
    UtilityFunctions::print("Start");
    _chunkSize = (1 << _minChunkSize);

    // PopulationLoDScale = _lod_distance / _feature_lod_distance * _octree_scale;
    //_sdf = new SphereSdf({0, -1024, 0}, 1024);// new PlaneSdf({0, 1, 0}, 0);
    _levelOfDetail =
        new VoxelLoD(_updateLoDAutomatically, _automaticUpdateDistance, _lodDistance, _lodCount, _lodPadding, this);
    //_levelOfDetail->UpdatePositionEvent.bind(this, &JarVoxelTerrain::build);
    _meshComputeScheduler = new MeshComputeScheduler(12);

    _voxelRoot = new VoxelOctreeNode(_size);
    //_populationRoot = memnew(PopulationOctreeNode(_size));
    build();
}

void JarVoxelTerrain::process()
{
    _meshComputeScheduler->process(this);

    // if (!_modifySettingsQueue.empty())
    // {
    //     process_modify_queue();
    // }

    // process_chunk_queue(static_cast<float>(delta));
}

void printUniqueLoDValues(const std::vector<int> &lodValues)
{
    godot::String lodString = "Unique LoD values: ";
    for (const auto &lod : lodValues)
    {
        lodString += godot::String::num_int64(lod) + ", ";
    }
    // Remove the last comma and space
    if (lodValues.size() > 0)
    {
        lodString = lodString.substr(0, lodString.length() - 2);
    }
    UtilityFunctions::print(lodString);
}

void JarVoxelTerrain::build()
{
    if (_isBuilding)
        return;
    _isBuilding = true;

    //_meshComputeScheduler->clear_queue();
    _voxelRoot->build(this);
    _isBuilding = false;

    // std::thread([this]() { _worldBiomes->update_texture(_levelOfDetail->get_camera_position()); }).detach();
    UtilityFunctions::print("Done Building.");
    UtilityFunctions::print(_voxelRoot->get_count());

    std::vector<int> uniqueLoDValues;
    _voxelRoot->populateUniqueLoDValues(uniqueLoDValues);
    printUniqueLoDValues(uniqueLoDValues);
}

void JarVoxelTerrain::process_chunk_queue(float delta)
{
    // if (_updateChunkCollidersQueue.empty())
    //     return;

    // int chunksPerSecond = _updateChunkCollidersQueue.size();
    // float max = std::max(chunksPerSecond * delta, 1.0f);

    // for (int i = 0; i < std::min(max, static_cast<float>(_updateChunkCollidersQueue.size())); i++)
    // {
    //     Chunk *chunk = _updateChunkCollidersQueue.front();
    //     _updateChunkCollidersQueue.pop();
    //     chunk->update_collision_mesh();
    // }
}

void JarVoxelTerrain::generate_epsilons()
{
    int numElements = _size + 1;

    std::vector<int> sizes(numElements);
    for (int i = 0; i < numElements; i++)
    {
        sizes[i] = 1 << i;
    }

    _voxelEpsilons.resize(sizes.size());
    for (int i = 0; i < sizes.size(); i++)
    {
        int size = sizes[i];
        float x = size * _octreeScale;
        _voxelEpsilons[i] = 1.75f * x;
    }
}

// void JarVoxelTerrain::process_modify_queue()
// {
//     if (_isBuilding)
//         return;
//     _isBuilding = true;
//     std::thread([this]() {
//         if (!_modifySettingsQueue.empty())
//         {
//             auto &settings = _modifySettingsQueue.front();
//             _modifySettingsQueue.pop();
//             _voxelRoot->modify_density_in_bounds(*this, settings);
//             //_populationRoot->remove_population(settings);
//         }
//         _isBuilding = false;
//     }).detach();
// }

void JarVoxelTerrain::sphere_edit_terrain(const Vector3 &position, float radius, float change)
{
    glm::vec3 pos = glm::vec3(position.x, position.y, position.z);
    Vector3 rad = Vector3(1.0f, 1.0f, 1.0f) * radius;
    // _modifySettingsQueue.push(
    //     ModifySettings{new SphereSdf(pos, radius), *(Bounds(pos - rad, rad * 2.0f).grow(_octreeScale * 2)),
    //                    change > 0 ? new SmoothUnionOperation : new SmoothSubtractionOperation});
}

void JarVoxelTerrain::get_voxel_leaves_in_bounds(const Bounds &bounds, std::vector<VoxelOctreeNode *> &nodes) const
{
    _voxelRoot->get_voxel_leaves_in_bounds(this, bounds, nodes);
}
