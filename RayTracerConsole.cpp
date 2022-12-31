#include <vector>
#include <iostream>
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>
#include "types.h"
#include "node.h"
#include "camera.h"
#include "surface.h"
#include "raytra_parser.h"
#include "raytracer.h"
#include "segfault_handler.h"
#include "light.h"
#include "bvh_node.h"
#include "surface_list.h"

using namespace RT::core;
using namespace std;
namespace po = boost::program_options;

bool ParseArguments(int argc, char** argv, std::string* input_scene_name,
    std::string* output_name, int* samples_per_pixel, int* shadow_samples) {
    po::options_description desc("options");
    try {
        desc.add_options()
            ("help,h", "print usage")
            ("input_scene,s",
                po::value(input_scene_name)->required(),
                "Input scene file")
            ("output,o",
                po::value(output_name)->required(),
                "Output name")
            ("samples_per_pixel,a",
                po::value(samples_per_pixel)->required(),
                "Samples per pixel")
            ("shadow_samples,d",
                po::value(shadow_samples)->required(),
                "Shadow Samples");

        // parse arguments
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            cout << desc << endl;
            return false;
        }
        po::notify(vm);
    }
    catch (std::exception& e) {
        cout << desc << endl;
        spdlog::error("{}", e.what());
        return false;
    }
    catch (...) {
        cout << desc << endl;
        spdlog::error("Invalid arguments");
        return false;
    }
    return true;
}


int
main(int argc, char** argv)
{
    utils::InstallSegfaultHandler();
    srand(123543);

    // parse command line arguments
    string input_scene_name, output_name;
    int samples_per_pixel;
    int shadow_samples;
    if (!ParseArguments(argc, argv, &input_scene_name, &output_name, &samples_per_pixel, &shadow_samples))
        return -1;

    // parse and render raytra scene
    Vec2i image_size;
    Surface::Ptr scene;
    vector<Light::Ptr> lights;
    Camera::Ptr camera;
    if (!RaytraParser::ParseFile(input_scene_name, scene, lights, camera,
        image_size, shadow_samples) || !scene || !camera || image_size[0] <= 0 ||
        image_size[1] <= 0) {
        spdlog::error("Failed to parse scene file.");
        return -1;
    }

    // render scene
    SurfaceList::Ptr list = dynamic_pointer_cast<SurfaceList>(scene);
    BVHNode::Ptr root;
    BVHNode::Ptr sc = root->BuildBVH(list->GetSurfaces());

    RayTracer rt;
    rt.SetNumSamplesPerPixel(samples_per_pixel);
    rt.SetImageHeight(static_cast<uint>(image_size[1]));
    rt.Render(sc, lights, camera);

    // save rendered image to file
    rt.WriteImage(output_name, 2);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
