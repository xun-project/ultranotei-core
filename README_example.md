# Ultranote Infinity Mobile App

UltraNote Infinity is a secure encrypted messaging and billing system with support for up to 100MB file transfers, wallet transactions, and comprehensive notification system.

## Features

### 🔐 Security & Authentication
- End-to-end encrypted messaging
- Two-factor authentication (2FA) support
- OTP (One-Time Password) verification
- Biometric authentication (Touch ID/Face ID)
- Secure wallet transactions

### 💬 Messaging
- Encrypted chat rooms and private messages
- File sharing up to 100MB
- Real-time message delivery
- Message status indicators (sent, delivered, read)
- Group chat support

### 💰 Wallet & Transactions
- Multi-currency wallet support (USD, BTC)
- Deposit and withdrawal functionality
- Transaction history with date-based grouping
- Billing and invoice management
- Secure payment processing

### 📱 Notifications
- Push notifications with custom sounds
- Notification actions (reply, mark read, view, delete)
- Deep linking to specific app sections
- Platform-specific notification handling (Android/iOS)
- Permission management

### 🎨 User Experience
- Responsive UI with dark/light theme support
- Loading states and error handling
- Empty state guidance
- Form validation with inline feedback
- Internationalization support

## Technical Stack

- **Framework**: Flutter 3.13.9
- **Language**: Dart 3.1.5
- **State Management**: Provider + Mixins
- **HTTP Client**: Dio with interceptors
- **Database**: Hive for local storage
- **Notifications**: Flutter Local Notifications
- **File Upload**: Cloudinary integration
- **Real-time**: Socket.io for WebSocket communication

## Project Structure

```
lib/
├── controller/          # Business logic and controllers
│   ├── logic/          # Application logic
│   ├── service/        # API and external services
│   └── mixins/         # Reusable functionality mixins
├── model/              # Data models and DTOs
│   ├── Dtos/          # Data Transfer Objects
│   ├── data/          # Local data storage
│   └── models/        # Domain models
├── view/               # UI components
│   ├── screen/        # Full screens
│   ├── tab/           # Tab views
│   └── widget/        # Reusable widgets
└── utils/              # Utilities and helpers
    ├── app_theme.dart # Theme configuration
    └── *.dart         # Various utility classes
```

## Getting Started

### Prerequisites

- Flutter SDK 3.13.9 or later
- Dart 3.1.5 or later
- Android Studio or VS Code with Flutter extension
- iOS development tools (for iOS builds)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/xun-project/ultranotei-mobile.git
   cd ultranotei-mobile
   ```

2. **Install dependencies**
   ```bash
   flutter pub get
   ```

3. **Run the app**
   ```bash
   flutter run
   ```

### Building for Production

**Android APK:**
```bash
flutter clean
flutter pub get
flutter build apk --release
```

**Android App Bundle:**
```bash
flutter build appbundle --release
```

**iOS Build:**
```bash
flutter build ios --release
```

## API Integration

The app communicates with the Ultranote backend API. Key endpoints include:

- **Authentication**: `/signin`, `/user/signup`, `/otp/:token`
- **Wallet**: `/wallets/my-wallet`, `/wallets/transactions`
- **Messages**: `/messages/:chatId`, `/wallets/messages`
- **2FA**: `/user/change2fa`, `/user/auth2FATMP`, `/user/auth2FAConfirm`
- **Notifications**: Integrated with FCM and local notifications

## Configuration

### Environment Variables

Create a `.env` file in the root directory:

```env
API_BASE_URL=https://cloud.ultranote.org/api
SOCKET_URL=https://cloud.ultranote.org
CLOUDINARY_CLOUD_NAME=your_cloud_name
CLOUDINARY_API_KEY=your_api_key
```

### Android Configuration

Update `android/app/src/main/AndroidManifest.xml` with proper permissions and intent filters.

### iOS Configuration

Update `ios/Runner/Info.plist` with required permissions and background modes.

## Testing

### Unit Tests
```bash
flutter test
```

### Integration Tests
```bash
flutter test integration_test/
```

### Notification Testing
```bash
flutter run test_notification_sound.dart
```

## Documentation

- **Project Overview**: `docs/project_completion_overview.md`
- **Notification System**: `NOTIFICATION_SYSTEM_GUIDE.md`
- **API Service**: `docs/project_analysis_and_improvements.md`

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Support

For support and questions:
- Check the documentation files
- Review existing issues on GitHub
- Contact the development team

## License

This project is proprietary software. All rights reserved.

## Version Information

- **Current Version**: 1.0.0
- **Flutter Version**: 3.13.9
- **Dart Version**: 3.1.5
- **Last Updated**: September 2025
