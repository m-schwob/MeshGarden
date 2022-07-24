/*import 'package:iot_firestore_flutter_app/model/Sensor.dart';

class Node {
  int? nodeID;
  String? nickname;
  bool? bridge;
  bool? active;
  bool? configured;
  List<Sensor>? sensors;

  Node({this.nodeID,
    this.nickname,
    this.bridge,
    this.active,
    this.configured,
    this.sensors});

  Node.fromJson(Map<String, Object?> json)
      : this(
        nodeID: json['nodeID'] as int,
        nickname: json['nickname'] as String,
        active: json['active'] as bool,
        configured: json['configured'] as bool,
        sensors:(json['sensors']! as List).cast<Sensor>(),
        // sensors:json['sensors'] as List<Sensor>,
  );

  Map<String, Object?> toJson() {
    return {
      'nodeID': nodeID,
      'nickname': nickname,
      'bridge': bridge,
      'active': active,
      'configured': configured,
      'sensors': sensors,
    };
  }

}*/
