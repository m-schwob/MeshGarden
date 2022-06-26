
class Sensor {
  String? hardware_info;
  Map<String, String>? pinout;
  int? sample_interval;
  int? sensor_id;
  List<String>? sensor_type;
  List<String>? units;

  Sensor({
    this.hardware_info,
    this.pinout,
    this.sample_interval,
    this.sensor_id,
    this.sensor_type,
    this.units,
  });

  Sensor.fromJson(Map<String, Object?> json)
    : this(
      hardware_info: json['hardware_info'] as String,
      pinout:(json['pinout']! as Map).cast<String,String>(),
      // pinout: json['pinout'] as Map<String,String>,
      sample_interval: json['sample_interval'] as int,
      sensor_id: json['sensor_id'] as int,
      sensor_type:(json['sensor_type']! as List).cast<String>(),
      // sensor_type: json['sensor_type'] as List<String>,
      units:(json['units']! as List).cast<String>(),
      // units: json['units'] as List<String>,
    );

  Map<String, Object?> toJson() {
    return {
      'hardware_info': hardware_info,
      'pinout': pinout,
      'sample_interval': sample_interval,
      'sensor_id': sensor_id,
      'sensor_type': sensor_type,
      'units': units,
    };
  }

// static List<SubCategory> fromJsonArray(List<dynamic> jsonArray) {
//   List<SubCategory> subCategoriesFromJson = [];
//
//   jsonArray.forEach((jsonData) {
//     subCategoriesFromJson.add(SubCategory.fromJson(jsonData));
//   });
//
//   return subCategoriesFromJson;
// }

}
