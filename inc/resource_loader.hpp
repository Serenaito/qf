#ifndef RESOURCE_LOADED_HPP
#define RESOURCE_LOADED_HPP

#include <vector>
constexpr int resource_name_size = 32;
constexpr int resource_path_size = 64;
class resource_loader
{
public:
    struct model
    {
        char name[resource_name_size];
        int x;
        int y;
        model(){};
        ~model(){};
    };
    static resource_loader& get_instance()
    {
        static resource_loader ins;
        return ins;
    }
    bool initialize();
    void release();
    const std::vector<model>& get_model_list();
    const char* get_system_tray_icon_path();
    const model* get_current_model();
    bool update_current_model(const char* name);
    bool update_current_model(uint32_t index);
    bool update_current_model_position(int x,int y);
    int  get_current_model_index() const;
    const char* get_config_path();
    bool moveable();
    void set_moveable(bool m);
    bool is_top();
    void set_top(bool top);
private:
    bool is_init;
    bool top;
    bool move;
    std::vector<model> model_list;
    char system_tray_icon_path[64];
    const model* current_model;
    int current_model_index;
    void* json_root;
    resource_loader() {is_init = false;};
    ~resource_loader();
};



#endif
