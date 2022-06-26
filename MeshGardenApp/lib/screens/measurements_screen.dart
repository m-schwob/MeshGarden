import 'package:flutter/material.dart';
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/model/sensor_measurements.dart';
import 'package:iot_firestore_flutter_app/widgets/measurement_card.dart';
import '../const/custom_colors.dart';
import 'package:iot_firestore_flutter_app/const/image_path.dart';
class MeasurementsScreen extends StatefulWidget {
  const MeasurementsScreen({
    Key? key,
    required this.nodeId,required this.hasMeasurements
  }) : super(key: key);

  final String nodeId;
  final bool hasMeasurements;

  @override
  _MeasurementsScreenState createState() => _MeasurementsScreenState();
}

class _MeasurementsScreenState extends State<MeasurementsScreen> {

  final Stream<QuerySnapshot> _measureStream =
      FirebaseFirestore.instance.collection('Measurements').snapshots();


  @override
  Widget build(BuildContext context) {
    if (!widget.hasMeasurements) {
      return Scaffold(
        appBar: AppBar(
          backgroundColor: kMainBG,
          elevation: 10.0,
          leading: IconButton(
            onPressed: () {
              Navigator.pop(context);
            },
            icon: Image(
              width: 24,
              color: Colors.white,
              image: AssetImage('assets/images/back_arrow.png'),
            ),
          ),
          title: Text("Measurements", style: kBodyText2),
        ),
        body: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Center(
                child:
                Text("No Measurements", style: kBodyText2),
              ),
              Center(
                child:
                  Text("for this plant", style: kBodyText2),
              ),
              Center(
                child: Image(
                  // width: 60,
                  image: AssetImage(
                    CryingBabyPlant,
                  ),
                ),
              ),
            ]),
      );
    } else {
      return Scaffold(
        appBar: AppBar(
          backgroundColor: kMainBG,
          elevation: 10.0,
          leading: IconButton(
            onPressed: () {
              Navigator.pop(context);
            },
            icon: Image(
              width: 24,
              color: Colors.white,
              image: AssetImage('assets/images/back_arrow.png'),
            ),
          ),
          title: Text("Measurements", style: kBodyText2),
        ),
        body: StreamBuilder<QuerySnapshot>(
          stream: _measureStream,
          builder: (BuildContext context,
              AsyncSnapshot<QuerySnapshot> snapshot) {
            if (snapshot.hasError) {
              return Center(
                child: Text(snapshot.error.toString()),
              );
            }

            if (!snapshot.hasData) {
              return const Center(child: CircularProgressIndicator());
            }

            Map<String, dynamic> node_sensor_data_map = {};

            snapshot.data!.docs.forEach((node_document) {
              // print(widget.nodeId + "\n");
              // print(node_document.id + "------\n");
              if (node_document.id.trim() ==
                  2989123787.toString().trim()) { //Todo change id to widget.nodeID
                node_sensor_data_map =
                node_document.data()! as Map<String, dynamic>;
              }
            });



            List<SensorMeasurements> measurements_list = [];

            // print(node_sensor_data_map);
            // TODO temp and remove use below version code
            // int i=0;
            // node_sensor_data_map.forEach((key, types) {
            //   if( key == "1" ) {
            //     Map<String, dynamic> types_map = types as Map<String, dynamic>;
            //     types_map.forEach((measure_type, measure_att) {
            //       if(measure_type =="Air Humidity") {
            //         SensorMeasurements measures = SensorMeasurements.fromJson(
            //             measure_att, measure_type);
            //         measurements_list.add(measures);
            //         i++;
            //       }
            //     });
            //   }
            // });


            node_sensor_data_map.forEach((key, types) {
              Map<String, dynamic> types_map = types as Map<String, dynamic>;
              types_map.forEach((measure_type, measure_att) {
                SensorMeasurements measures = SensorMeasurements.fromJson(measure_att, measure_type);
                measurements_list.add(measures);
              });
            });



            // Timestamp? time = measurements_list[0].newSample!.time;
            // print(time);

            return Scaffold(
              body: Padding(
                padding: const EdgeInsets.only(
                    left: 16, right: 16, top: 20, bottom: 30),
                child: ListView.builder(
                  itemCount: measurements_list.length,
                  itemBuilder: (BuildContext ctx, int index) {

                    return Container(
                            child: Center(
                                  child: Column(
                                    crossAxisAlignment: CrossAxisAlignment.center,
                                    children: [
                                      MeasurementCard(
                                        value: measurements_list[index].newSample?.value,
                                        date_time: measurements_list[index].newSample?.time?.toDate().add(Duration(hours: 3)),
                                        unit: measurements_list[index].units,
                                        name: measurements_list[index].type,
                                        assetImage: measurements_list[index].imagePath,
                                        // trendData: rhList!,
                                        // linePoint: Colors.blueAccent,
                                      ),
                                      SizedBox(
                                        height: 20,
                                      )
                                    ],
                                  ),
                                ),
                    );
                  },
                ),
              ),
            );
          },
        ),
      );
    }
  }
}

