/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 * limitations under the License.
 */
package no.ntnu.osnap.social;

import android.app.Service;
import android.content.*;
import android.os.*;

import android.util.Log;

/**
 * oSNAP class representing a Social service, which is an Android service that
 * handles prototypes requests. To use it, extend it and implement
 * {@link #handleRequest(Request )} and {@link #handlePostRequest(Request )}.
 * SocialService will invoke these methods to obtain a {@link Response}
 * to send back.
 *
 * @author Emanuele 'lemrey' Di Santo
 */
public class SocialService extends Service {

	/**
	 * Used for logging purposes.
	 */
	private static final String TAG = "Social-Service";
	
	/**
	 * The SocialService name, as published to prototypes.
	 */	
	protected String mName = "Social-Service";
	
	/**
	 * Target we publish for clients to send messages to IncomingHandler.
	 */
	private final Messenger mMessenger = new Messenger(new IncomingHandler());

	/**
	 * Called by the system when the service is first started. Subclasses must
	 * call the superclass implementation if overriding this method.
	 */
	@Override
	public void onCreate() {
		Log.d(TAG, "Started");

		registerReceiver(new BroadcastRcv(),
				new IntentFilter("android.intent.action.SOCIAL"));
	}

	/**
	 * When binding to the service, we return an interface to our messenger for
	 * sending messages to the service.
	 */
	@Override
	public IBinder onBind(Intent intent) {
		return mMessenger.getBinder();
	}

	/**
	 * Sets the Social service name, which is used by prototypes to identify the
	 * service.
	 *
	 * @param name the name to be published to prototypes
	 */
	public void setServiceName(String name) {
		mName = name;
	}

	/**
	 * Handles incoming messages from prototypes.
	 */
	private class IncomingHandler extends Handler {

		@Override
		public void handleMessage(Message msg) {

			Log.d(TAG, "handleMessage()");

			Message reply;
			Response response = null;

			switch (msg.what) {

				case 1: { // Request (GET)
					Log.d(TAG, "Replying a request(GET)");
					Request req = Request.fromMessage(msg);
					response = handleRequest(req);
				}
				break;

				case 2: { // Request (POST)
					Log.d(TAG, "Replying a request(POST)");
					Request req = Request.fromMessage(msg);
					response = handlePostRequest(req);
				}
				break;

				default:
					super.handleMessage(msg);
			}

			// send back the response
			if (response != null) {
				reply = Message.obtain(null, 1, response.getBundle());
				reply.arg1 = msg.arg1; //copy the id
				try {
					msg.replyTo.send(reply);
				} catch (RemoteException ex) {
					Log.e(TAG, ex.toString());
				}
			}
		}
	}

	/**
	 * This method is invoked by the SocialService to handle incoming requests
	 * to fetch data from the social network. This method should return an
	 * appropriate {@link Response} object to be returned to the Prototype who
	 * made the request. Only non-{@code null} {@link Response} objects are sent
	 * back to the Prototype. Only one request is processed at a time.
	 *
	 * @param req the {@link Request} to be carried out
	 * @return a {@link Response} containing the data requested
	 */
	protected Response handleRequest(Request req) {
		return null;
	}

	/**
	 * Called by the SocialService implementation to handle incoming requests
	 * to send data to the social network.
	 * @param req the {@link Request} to be carried out
	 * @return a {@link Response}
	 */
	protected Response handlePostRequest(Request req) {
		return null;
	}

	/**
	 * Receives and replies discovery broadcasts sent by prototypes.
	 */
	private class BroadcastRcv extends BroadcastReceiver {

		// used for logging purposes
		private final String TAG = "Broadcast-Rcv";

		@Override
		public void onReceive(Context context, Intent intent) {

			if (intent.getAction().equals("android.intent.action.SOCIAL")) {

				Log.d(TAG, "Discovered");
				Bundle bundle = new Bundle();
				Bundle extras = intent.getExtras();
				Messenger messenger = extras.getParcelable("replyTo");

				try {
					/*
					 * craft a Message object containg our Messenger and service
					 * name and send it back to the Messenger bundled with the
					 * broadcast
					 */
					bundle.putString("name", mName);
					Message msg = Message.obtain(null, 0, bundle);
					msg.replyTo = SocialService.this.mMessenger;
					messenger.send(msg);
				} catch (RemoteException ex) {
					Log.d(TAG, ex.toString());
				}
			}
		}
	}
}
