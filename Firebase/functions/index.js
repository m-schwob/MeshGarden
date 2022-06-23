const functions = require("firebase-functions");
const admin = require("firebase-admin");
admin.initializeApp();


exports.onNodeWrite = functions.firestore.document('/Nodes/{node_id}').onWrite(async (change, context) => {
    const node_id = context.params.node_id;

    await update_changes_collection(change, node_id);
    await update_measurements_collection(change, node_id);
});

async function update_changes_collection(change, node_id) {
    if (change.after.exists) {
        const data = change.after.data();
        const doc = admin.firestore().collection('Changes').doc(node_id);
        doc.set({ 'config': JSON.stringify(data) });
        functions.logger.log("node " + node_id + " changed");
    } else {
        functions.logger.log("node " + node_id + " deleted");
    }
}

async function update_measurements_collection(change, node_id) {
    // get sensors ids from node document if exist
    var node_sensors_ids = [];
    if (change.after.exists) {
        node_sensors = await change.after.get('sensors');
        if (node_sensors) {
            node_sensors_ids = Object.keys(node_sensors);
        }
    }
    functions.logger.log(`node ${node_id} document sensors list: [${node_sensors_ids}]`);

    const meas_doc = await admin.firestore().collection('Measurements').doc(node_id).get();
    var meas_sensors_ids = [];
    if (meas_doc.exists) {
        node_sensors = meas_doc.data();
        if (node_sensors) {
            meas_sensors_ids = Object.keys(node_sensors);
        }
    }
    functions.logger.log(`node ${node_id} measurements document sensors list: [${meas_sensors_ids}]`);
}


