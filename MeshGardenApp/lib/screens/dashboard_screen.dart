/**
import 'package:cloud_firestore/cloud_firestore.dart';
import 'package:iot_firestore_flutter_app/auth_helper.dart';
import 'package:iot_firestore_flutter_app/const/custom_styles.dart';
import 'package:iot_firestore_flutter_app/model/Sensor.dart';
import 'package:iot_firestore_flutter_app/route/routing_constants.dart';
import 'package:flutter/material.dart';

class DashboardScreen extends StatefulWidget {
  const DashboardScreen({Key? key}) : super(key: key);

  @override
  _DashboardScreenState createState() => _DashboardScreenState();
}

class _DashboardScreenState extends State<DashboardScreen> {
  List<double>? tempList;
  List<double>? rhList;

  static String collectionName = 'TestMeasurements';
  final sensorRef = FirebaseFirestore.instance
      .collection(collectionName)
      .withConverter<Sensor>(
        fromFirestore: (snapshots, _) => Sensor.fromJson(snapshots.data()!),
        toFirestore: (movie, _) => movie.toJson(),
      );

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: StreamBuilder<QuerySnapshot<Sensor>>(
      stream: sensorRef.snapshots(),
      builder: (context, snapshot) {
        if (snapshot.hasError) {
          return Center(
            child: Text(snapshot.error.toString()),
          );
        }

        if (!snapshot.hasData) {
          return const Center(child: CircularProgressIndicator());
        }

        final data = snapshot.requireData;

        if (tempList == null) {
          tempList = List.filled(5, data.docs.first.data().temperature.toDouble(),
              growable: true);
        } else {
          tempList!.add(data.docs.first.data().temperature.toDouble());
          tempList!.removeAt(0);
        }

        if (rhList == null) {
          rhList =
              List.filled(5, data.docs.first.data().humidity.toDouble(), growable: true);
        } else {
          rhList!.add(data.docs.first.data().humidity.toDouble());
          rhList!.removeAt(0);
        }

        return Padding(
          padding:
              const EdgeInsets.only(left: 16, right: 16, top: 40, bottom: 30),
          child: CustomScrollView(slivers: [
            SliverFillRemaining(
              hasScrollBody: false,
              child: Column(
                children: [
                  Flexible(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Padding(
                          padding: const EdgeInsets.all(8.0),
                          child: Text(
                            //collectionName,
                            "Taub Plant",
                            style: kHeadline,
                          ),
                        ),
                        Padding(
                          padding: const EdgeInsets.all(8.0),
                          child: Text(
                            //data.docs.first.id,
                            "Sensor 1",
                            style: kHeadline,
                          ),
                        ),
                        SizedBox(
                          height: 30,
                        ),
                        Center(
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.center,
                            children: [
                              MySensorCard(
                                value: data.docs.first.data().humidity,
                                unit: '%',
                                name: 'Air Humidity',
                                assetImage: AssetImage(
                                  'assets/images/humidity_icon.png',
                                ),
                                trendData: rhList!,
                                linePoint: Colors.blueAccent,
                              ),
                              SizedBox(
                                height: 20,
                              ),
                              MySensorCard(
                                value: data.docs.first.data().temperature,
                                unit: '\'C',
                                name: 'Air Temperature',
                                assetImage: AssetImage(
                                  'assets/images/temperature_icon.png',
                                ),
                                trendData: tempList!,
                                linePoint: Colors.redAccent,
                              )
                            ],
                          ),
                        ),
                      ],
                    ),
                  ),
                  Row(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(
                        "Sign out of Firebase? ",
                        style: kBodyText,
                      ),
                      GestureDetector(
                        onTap: _signOut,
                        child: Text(
                          "Sign Out",
                          style: kBodyText.copyWith(
                            color: Colors.white,
                          ),
                        ),
                      ),
                    ],
                  ),
                  SizedBox(
                    height: 20,
                  ),
                ],
              ),
            ),
          ]),
        );
      },
    ));
  }

  _signOut() async {
    await AuthHelper.signOut();
    Navigator.pushNamedAndRemoveUntil(
        context, SplashScreenRoute, (Route<dynamic> route) => false);
  }
}
    */
