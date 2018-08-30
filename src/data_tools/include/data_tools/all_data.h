#ifndef ALL_DATA_H
#define ALL_DATA_H

#include <data_tools/navi_data.h>
#include <eigen3/Eigen/Dense>
#include <boost/filesystem.hpp>
//#include <cereal/types/vector.hpp>

struct all_mbes_ping {

    using PingsT = std::vector<all_mbes_ping, Eigen::aligned_allocator<all_mbes_ping> >;

    unsigned int id_;
    std::string time_string_; // readable time stamp string
    long long time_stamp_; // posix time stamp
	double heading_;
	double sound_vel_;
	double transducer_depth_;
    std::vector<float> reflectivities; // amplitudes
    std::vector<Eigen::Vector3d, Eigen::aligned_allocator<Eigen::Vector3d> > beams; // all 3d points in swath
    bool first_in_file_;

	/*
	template <class Archive>
    void serialize( Archive & ar )
    {
        ar(CEREAL_NVP(id_), CEREAL_NVP(time_string_), CEREAL_NVP(time_stamp_),
		   CEREAL_NVP(reflectivities), CEREAL_NVP(beams), CEREAL_NVP(first_in_file_));
    }
	*/

};

struct all_nav_entry {

    using EntriesT = std::vector<all_nav_entry, Eigen::aligned_allocator<all_nav_entry> >;

    unsigned int id_;
    std::string time_string_; // readable time stamp string
    long long time_stamp_; // posix time stamp
    double lat_;
    double long_;
	double heading_;
	double course_over_ground_;
    bool first_in_file_;

	/*
	template <class Archive>
    void serialize( Archive & ar )
    {
        ar(CEREAL_NVP(id_), CEREAL_NVP(time_string_), CEREAL_NVP(time_stamp_),
		   CEREAL_NVP(lat_), CEREAL_NVP(long_), CEREAL_NVP(heading_),
		   CEREAL_NVP(course_over_ground_), CEREAL_NVP(first_in_file_));
    }
	*/

};

//template <typename ReturnType>
//std::vector<ReturnType, Eigen::aligned_allocator<ReturnType> > parse_file(const boost::filesystem::path& path);
template <>
all_mbes_ping::PingsT parse_file<all_mbes_ping>(const boost::filesystem::path& path);

template <>
all_nav_entry::EntriesT parse_file<all_nav_entry>(const boost::filesystem::path& path);

#endif // ALL_DATA_H
