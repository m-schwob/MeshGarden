// import 'package:cloud_firestore/cloud_firestore.dart';
// import 'package:iot_firestore_flutter_app/model/Node.dart';
//
// class NodeService {
//
//   FirebaseFirestore? _instance;
//
//   List<Node> _nodes = []; //List of nodes
//
//   List<Node> getNodes() {
//     return _nodes;
//   }
//
//   Future<void> getNodesCollectionFromFirestore() async {
//     _instance = FirebaseFirestore.instance;
//
//     CollectionReference nodes_collection = _instance!.collection('TestNodes');
//
//     // Get docs from collection reference
//     QuerySnapshot nodes_querySnapshot = await nodes_collection.get();
//
//     List<dynamic> nodes_list = nodes_querySnapshot.docs;
//
// }